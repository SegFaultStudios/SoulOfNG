#ifndef PACKET_PARSER_HPP
#define PACKET_PARSER_HPP

#include <SFML/Network.hpp>

#include "NetworkPackets.hpp"
#include "Packet.hpp"

#include <functional>
#include <memory>

class PacketParser
{
public:
    using PacketReceivedFunction = std::function<void(std::unique_ptr<NetworkPacket> packet, PacketType packetType)>;

    void parsePacket(sf::Packet& packet);

    void setOnParsedPacket(const PacketReceivedFunction& function);
private:
    void sendPacket(std::unique_ptr<NetworkPacket> packet);

    PacketReceivedFunction m_onPacketParsed{nullptr};
};

#endif //PACKET_PARSER_HPP