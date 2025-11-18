#include "PacketParser.hpp"

void PacketParser::parsePacket(sf::Packet& packet)
{
    NetworkPacketHeader header;

    packet >> header.type;
    packet >> header.size;

    PacketType packetType = static_cast<PacketType>(header.type);

    if(packetType == PacketType::PING || packetType == PacketType::PONG)
    {
        auto receivedPacket = std::make_unique<PingPongPacket>(header);
        packet >> receivedPacket->timestamp;
        sendPacket(std::move(receivedPacket));
        return;
    }

    switch(packetType)
    {
    }
}

void PacketParser::sendPacket(std::unique_ptr<NetworkPacket> packet)
{
    if(!m_onPacketParsed)
        return;

    PacketType packetType = static_cast<PacketType>(packet->header.type);

    m_onPacketParsed(std::move(packet), packetType);
}

void PacketParser::setOnParsedPacket(const PacketParser::PacketReceivedFunction& function)
{
    m_onPacketParsed = function;
}