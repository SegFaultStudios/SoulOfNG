#ifndef NETWORK_TYPE_HPP
#define NETWORK_TYPE_HPP

#include <cstdint>
#include <string>

enum class PacketType : uint16_t
{
    PING = 1,
    PONG = 2,
    GET_LOBBIES = 3,
    RESPONSE_GET_LOBBIES = 4,
    CREATE_LOBBY = 5,
    RESPONSE_CREATE_LOBBY = 6,
    JOIN_LOBBY = 7,
    RESPONSE_JOIN_LOBBY = 8,
    UPDATE_LOBBY = 9,
    LEAVE_LOBBY = 10,
    INSIDE_LOBBY_DATA_UPDATE = 11
};

namespace PacketTypeUtil
{
    inline std::string fromTypeToString(PacketType packetType)
    {
        switch (packetType)
        {
        case PacketType::PING:
            return "PING";
        case PacketType::PONG:
            return "PONG";
        case PacketType::GET_LOBBIES:
            return "GET_LOBBIES";
        case PacketType::RESPONSE_GET_LOBBIES:
            return "RESPONSE_GET_LOBBIES";
        default:
            return "";
        }
    }
} // namespace PacketTypeUtils

#endif // NETWORK_TYPE_HPP