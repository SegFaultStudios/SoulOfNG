#ifndef STEAM_CLIENT_NETWORK_HPP
#define STEAM_CLIENT_NETWORK_HPP

#include "Network/SteamBackend.hpp"

#include "Signal.hpp"

#include <cstdint>
#include <string>
#include <vector>

//For now, this is P2P steam client. Yeah, it sucks...
class SteamClientNetwork
{
public:
    SteamClientNetwork();

    enum class Role
    {
        NONE = 0,
        HOST = 1,
        CLIENT = 2
    };

    struct SteamLobbyData
    {
        std::string name;
        uint64_t id;
        uint64_t players;
        uint64_t maxPlayers;
    };

    struct SteamFriend
    {
        std::string name;
        uint32 avatarWidth, avatarHeight;   
        std::vector<uint8_t> avatarData;
        CSteamID id;
    };

    enum class FriendSearch : uint8_t
    {
        DEFAULT = 0,
        ONLINE = 1
    };

SIGNAL_NAMESPACE_BEGIN
    SIGNAL(roleChanged, Role);
    SIGNAL(lobbyCreated, bool);
    SIGNAL(foundLobbies, const std::vector<SteamLobbyData>&);
SIGNAL_NAMESPACE_END

    void clearSlots()
    {
        signals().foundLobbies.clearSlots();
        signals().lobbyCreated.clearSlots();
        signals().roleChanged.clearSlots();
    }

    STEAM_CALLBACK(SteamClientNetwork, onLobbyCreated, LobbyCreated_t, m_lobbyCreatedCallback);
    STEAM_CALLBACK(SteamClientNetwork, onLobbyEntered, LobbyEnter_t, m_lobbyEnteredCallback);
    STEAM_CALLBACK(SteamClientNetwork, onFoundLobbies, LobbyMatchList_t, m_lobbyFoundCallback);
    STEAM_CALLBACK(SteamClientNetwork, onLobbyUpdated, LobbyChatUpdate_t, m_lobbyUpdatedCallback);
    STEAM_CALLBACK(SteamClientNetwork, onJoinRequest, GameRichPresenceJoinRequested_t, m_joinRequestCallback);
    STEAM_CALLBACK(SteamClientNetwork, onLobbyJoinRequest, GameLobbyJoinRequested_t, m_lobbyJoinRequestCallback);

    std::vector<SteamFriend> getSteamFriends(FriendSearch friendSearch = FriendSearch::DEFAULT);

    std::string getName();

    void inviteFriend(CSteamID friendId);

    bool isConnectedToSteamServers() const;

    void requestLobbies();

    void setRole(Role role);
    Role getRole() const;

    void update();

    void createLobby();
    void joinLobby(CSteamID lobbyID);
private:
    Role m_role{Role::NONE};
    // std::vector<CSteamID> m_clients;
    // std::unordered_set<CSteamID> m_clients;
    CSteamID m_lobbyID;
    CSteamID m_hostID;

    static constexpr uint8_t MAX_PLAYERS_IN_LOBBY = 4;
};

#endif //STEAM_CLIENT_NETWORK_HPP