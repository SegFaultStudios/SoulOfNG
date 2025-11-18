#ifndef NETWORK_PACKET_HPP
#define NETWORK_PACKET_HPP

#include <cstdint>

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

#endif //NETWORK_PACKET_HPP