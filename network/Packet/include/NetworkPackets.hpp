#ifndef NETWORK_PACKET_HPP
#define NETWORK_PACKET_HPP

#include <cstdint>
#include <vector>
#include <string>

struct NetworkPacketHeader
{
    uint16_t type;
    uint16_t size;
};

struct NetworkPacket
{
    NetworkPacketHeader header;

    virtual ~NetworkPacket() = default;
};

struct PingPongPacket : public NetworkPacket
{
    int32_t timestamp;
};

struct GetLobbiesPacket : public NetworkPacket
{
    struct LobbyData
    {
        std::string lobbyName;
        uint32_t lobbyId;
        uint16_t playersAmount;
        uint16_t maxNumberOfPlayers;
    };

    uint16_t lobbiesSize;
    std::vector<LobbyData> lobbies;
};

struct CreateLobbyPacket : public NetworkPacket
{
    std::string lobbyName;
    uint16_t maxNumberOfPlayers;
    std::string adminName;
    bool isPrivate;
};

struct CreateLobbyResponsePacket : public NetworkPacket
{
    bool success;
};

struct JoinLobbyPacket : public NetworkPacket
{
    uint32_t lobbyId;
    std::string login;
    std::string password;
};

struct LobbyUpdatePacket : public NetworkPacket
{
    GetLobbiesPacket::LobbyData lobbyData;
};

struct JoinLobbyResponsePacket : public NetworkPacket
{
    enum JoinLobbyResponseType
    {
        FAILED = 0,
        SUCCESS = 1,
        NEED_TO_REGISTER = 2,  // Public lobby but need password and login to join
        FAILED_TO_REGISTER = 3 // Password or login are invalid
    };

    uint8_t response;
};

struct InsideLobbyDataUpdatePacket : public NetworkPacket
{
    struct Player
    {
        std::string name;
    };

    uint16_t playersSize;

    std::vector<Player> players;
};

#endif // NETWORK_PACKET_HPP