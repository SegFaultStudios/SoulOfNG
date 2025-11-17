#ifndef PACKET_HPP
#define PACKET_HPP

#include <SFML/Network.hpp>
#include <cstdint>
#include <cstring>

#include "Types.hpp"

struct PacketHeader
{
    uint16_t size;
    PacketType type;
};

struct Packet
{
public:
    Packet(PacketType type)
    {
        m_header.size = 0;
        m_header.type = type;

        write(static_cast<uint16_t>(m_header.type));
        write(m_header.size);
    }

    template<typename T>
    Packet& operator<<(const T& data)
    {
        m_packet << data;
        return *this;
    }

    // template<typename T>
    // Packet& operator>>(T& data)
    // {
    //     m_packet >> data;
    //     return *this;
    // }

    template<typename T>
    Packet& write(const T& value)
    {
        m_packet << value;
        return *this;
    }

    void writeRaw(const void* data, std::size_t size)
    {
        m_packet.append(data, size);
    }

    sf::Packet& finalize()
    {
        m_header.size = m_packet.getDataSize() - sizeof(PacketHeader);
        return m_packet;
    }

    const PacketHeader& header() const { return m_header; }
private:
    PacketHeader m_header;
    sf::Packet m_packet;
};

#endif //PACKET_HPP