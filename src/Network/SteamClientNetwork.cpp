#include "Network/SteamClientNetwork.hpp"
#include "Logger.hpp"

#include <iostream>
#include <sstream>

SteamClientNetwork::SteamClientNetwork() : 
m_lobbyCreatedCallback(this, &SteamClientNetwork::onLobbyCreated), 
m_lobbyEnteredCallback(this, &SteamClientNetwork::onLobbyEntered),
m_lobbyFoundCallback(this, &SteamClientNetwork::onFoundLobbies),
m_lobbyUpdatedCallback(this, &SteamClientNetwork::onLobbyUpdated),
m_joinRequestCallback(this, &SteamClientNetwork::onJoinRequest),
m_lobbyJoinRequestCallback(this, &SteamClientNetwork::onLobbyJoinRequest)
{
}

void SteamClientNetwork::onLobbyJoinRequest(GameLobbyJoinRequested_t* pCallbacks)
{
    std::string friendName = SteamFriends()->GetFriendPersonaName(pCallbacks->m_steamIDFriend);

    LOG_INFO(friendName + " invited you to lobby");

    SteamMatchmaking()->JoinLobby(pCallbacks->m_steamIDLobby);
}

std::string SteamClientNetwork::getName()
{
    CSteamID id = SteamUser()->GetSteamID();

    return SteamFriends()->GetFriendPersonaName(id);
}

void SteamClientNetwork::onJoinRequest(GameRichPresenceJoinRequested_t* pCallbacks)
{
    std::string connect = pCallbacks->m_rgchConnect;
    LOG_INFO(connect);

    // std::string prefix = "steam://joinlobby/";
    // if (connect.rfind(prefix, 0) != 0)
    //     return;

    // std::string content = connect.substr(prefix.size());
    // std::stringstream ss(content);

    // std::string appidStr, lobbyStr, userStr;
    // std::getline(ss, appidStr, '/');
    // std::getline(ss, lobbyStr, '/');
    // std::getline(ss, userStr, '/');

    // uint64 lobbyID = std::stoull(lobbyStr);

    // LOG_INFO("Extracted lobby id: " + std::to_string(lobbyID));

    // SteamMatchmaking()->JoinLobby(CSteamID(lobbyID));
}

void SteamClientNetwork::onLobbyUpdated(LobbyChatUpdate_t* pCallback)
{
    CSteamID userChanged = pCallback->m_ulSteamIDUserChanged;

    if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeEntered)
    {
        if (m_role == Role::HOST) 
        {
            // m_clients.insert(userChanged);
            std::cout << "Client joined: " << userChanged.ConvertToUint64() << std::endl;
        }
    }
    if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeLeft ||
        pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeDisconnected) 
    {
        if (m_role == Role::HOST)
        {
            // m_clients.erase(userChanged);
            std::cout << "Client left: " << userChanged.ConvertToUint64() << std::endl;
        }
    }
}

bool SteamClientNetwork::isConnectedToSteamServers() const
{
    return SteamUser()->BLoggedOn();
}

void SteamClientNetwork::requestLobbies()
{
    if(!SteamBackend::isInitialized())
        return;

    SteamMatchmaking()->AddRequestLobbyListStringFilter("game", "__SOUL_OF_NG__", k_ELobbyComparisonEqual);
    SteamMatchmaking()->RequestLobbyList();
}

void SteamClientNetwork::onFoundLobbies(LobbyMatchList_t* pCallback)
{
    std::cout << "Got " << pCallback->m_nLobbiesMatching << " lobbies\n";

    std::vector<SteamLobbyData> steamLobbiesData;

    for(uint32 lobbyIndex = 0; lobbyIndex < pCallback->m_nLobbiesMatching; ++lobbyIndex)
    {
        CSteamID lobbyId = SteamMatchmaking()->GetLobbyByIndex(lobbyIndex);

        std::string lobbyName = SteamMatchmaking()->GetLobbyData(lobbyId, "name");

        uint64_t numberOfPlayers = SteamMatchmaking()->GetNumLobbyMembers(lobbyId);

        SteamLobbyData lobbyData;
        lobbyData.id = lobbyId.ConvertToUint64();
        lobbyData.name = lobbyName;
        lobbyData.players = numberOfPlayers;
        lobbyData.maxPlayers = MAX_PLAYERS_IN_LOBBY;

        steamLobbiesData.push_back(lobbyData);
    }

    CALL_SIGNAL(foundLobbies, steamLobbiesData);
}

std::vector<SteamClientNetwork::SteamFriend> SteamClientNetwork::getSteamFriends(FriendSearch friendSearch)
{
    int count = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);

    if(count < 0)
    {
        LOG_WARNING("No friends found");
        return {};
    }

    std::vector<SteamFriend> friends;

    for(int i = 0; i < count; ++i)
    {
        CSteamID friendId = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);

        EPersonaState personaState = SteamFriends()->GetFriendPersonaState(friendId);

        if(static_cast<uint8_t>(friendSearch) & static_cast<uint8_t>(FriendSearch::ONLINE))
        {
            LOG_INFO("Looking for online friends");
            if(personaState == k_EPersonaStateOnline || personaState == k_EPersonaStateAway || 
            personaState == k_EPersonaStateSnooze || personaState == k_EPersonaStateLookingToTrade || 
            personaState == k_EPersonaStateLookingToPlay || personaState == k_EPersonaStateBusy)
            {
                LOG_INFO("Found online friend");
            }
            else
                continue;
        }
        else
            LOG_INFO("Looking for all friends");

        const char* name = SteamFriends()->GetFriendPersonaName(friendId);

        auto& steamFriend = friends.emplace_back();

        steamFriend.name = std::string(name);
        steamFriend.id = friendId;

        int avatar = SteamFriends()->GetLargeFriendAvatar(friendId);

        if(avatar != 1)
        {
            uint32 width, height;

            if (!SteamUtils()->GetImageSize(avatar, &width, &height))
            {
                LOG_WARNING("Failed to get avatar size");
                continue;
            }
            
            std::vector<uint8_t> data(width * height * 4);

            if (!SteamUtils()->GetImageRGBA(avatar, data.data(), data.size()))
            {
                LOG_WARNING("Failed to get avatar RGBA");
                continue;
            }

            steamFriend.avatarData = data;
            steamFriend.avatarHeight = height;
            steamFriend.avatarWidth = width;
        }
        else
            LOG_WARNING("No avatar");
    }

    return friends;
}

void SteamClientNetwork::inviteFriend(CSteamID friendId)
{
    SteamFriends()->InviteUserToGame(friendId, "Pidoras");
}

void SteamClientNetwork::onLobbyCreated(LobbyCreated_t* pCallback)
{
    if (pCallback->m_eResult != k_EResultOK)
    {
        std::cerr << "Failed to create lobby: " << pCallback->m_eResult << std::endl;
        CALL_SIGNAL(lobbyCreated, false);
        return;
    }

    m_lobbyID = pCallback->m_ulSteamIDLobby;
    m_hostID = SteamUser()->GetSteamID();
    m_role = Role::HOST;

    SteamMatchmaking()->SetLobbyData(m_lobbyID, "name", "DlyvernTestLobby");

    //!!!!! WE NEED TO CHANGE AS FUCKING FAST AS POSSIBLE
    SteamMatchmaking()->SetLobbyData(m_lobbyID, "game", "__SOUL_OF_NG__");

    std::cout << "Lobby created! ID: " << m_lobbyID.ConvertToUint64() << std::endl;

    CALL_SIGNAL(lobbyCreated, true);
}

void SteamClientNetwork::onLobbyEntered(LobbyEnter_t* pCallback)
{
    m_lobbyID = pCallback->m_ulSteamIDLobby;

    LOG_INFO("Lobby entered");

    if(m_role == Role::HOST)
    {
        LOG_DEBUG("Host");
        return;
    }

    if(m_role == Role::CLIENT)
    {
        LOG_WARNING("Steam client already in lobby. Something weird");
        return;
    }

    m_role = Role::CLIENT;

    m_hostID = SteamMatchmaking()->GetLobbyOwner(m_lobbyID);
    
    std::cout << "Joined lobby! Host: " << m_hostID.ConvertToUint64() << std::endl;
}

void SteamClientNetwork::setRole(SteamClientNetwork::Role role)
{
    if(m_role == role)
        return;
    
    m_role = role;

    CALL_SIGNAL(roleChanged, role);
}

void SteamClientNetwork::update()
{
    if(!SteamBackend::isInitialized())
        return;

    SteamAPI_RunCallbacks();
}

void SteamClientNetwork::createLobby()
{
    if(!SteamBackend::isInitialized())
        return;
    
    if(m_role == Role::HOST)
    {
        std::cerr << "Already host \n";
        return;
    }

    SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_PLAYERS_IN_LOBBY);
}

void SteamClientNetwork::joinLobby(CSteamID lobbyID)
{
    if(!SteamBackend::isInitialized())
        return;

    if(m_role == Role::HOST || m_role == Role::CLIENT)
    {
        std::cerr << "Client already connected to lobby\n";
        return;
    }

    SteamMatchmaking()->JoinLobby(lobbyID);
}

SteamClientNetwork::Role SteamClientNetwork::getRole() const
{
    return m_role;
}