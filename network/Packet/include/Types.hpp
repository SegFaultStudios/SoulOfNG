#ifndef NETWORK_TYPE_HPP
#define NETWORK_TYPE_HPP

#include <cstdint>

enum class PacketType : uint16_t
{
    PING = 1,
    PONG = 2,
    ASK_FOR_LOBBIES = 3,
    GET_LOBBIES = 4
};

#endif //NETWORK_TYPE_HPP