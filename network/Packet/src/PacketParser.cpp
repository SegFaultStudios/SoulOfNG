#include "PacketParser.hpp"
#include <iostream>

void PacketParser::parsePacket(sf::Packet &packet)
{
    NetworkPacketHeader header;

    packet >> header.type;
    packet >> header.size;

    PacketType packetType = static_cast<PacketType>(header.type);

    if (packetType == PacketType::PING || packetType == PacketType::PONG)
    {
        auto receivedPacket = std::make_unique<PingPongPacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->timestamp;
        sendPacket(std::move(receivedPacket));
        return;
    }

    switch (packetType)
    {
    case PacketType::RESPONSE_GET_LOBBIES:
    {
        auto receivedPacket = std::make_unique<GetLobbiesPacket>();
        receivedPacket->header = header;

        packet >> receivedPacket->lobbiesSize;

        receivedPacket->lobbies.reserve(receivedPacket->lobbiesSize);

        for (int index = 0; index < receivedPacket->lobbiesSize; ++index)
        {
            std::string lobbyName;
            uint32_t lobbyId;
            uint16_t playersAmount;
            uint16_t maxNumberOfPlayers;

            packet >> lobbyName;
            packet >> lobbyId;
            packet >> playersAmount;
            packet >> maxNumberOfPlayers;

            std::cout << "Lobby name: " << lobbyName << "(" << lobbyId << ") " << playersAmount << "/" << maxNumberOfPlayers << std::endl;

            GetLobbiesPacket::LobbyData lobbyData{
                .lobbyName = lobbyName,
                .lobbyId = lobbyId,
                .playersAmount = playersAmount,
                .maxNumberOfPlayers = maxNumberOfPlayers,
            };

            receivedPacket->lobbies.push_back(lobbyData);
        }

        sendPacket(std::move(receivedPacket));

        break;
    };

    case PacketType::GET_LOBBIES:
    {
        auto receivedPacket = std::make_unique<NetworkPacket>();
        receivedPacket->header = header;
        sendPacket(std::move(receivedPacket));
        break;
    }

    case PacketType::CREATE_LOBBY:
    {
        auto receivedPacket = std::make_unique<CreateLobbyPacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->lobbyName;
        packet >> receivedPacket->maxNumberOfPlayers;
        packet >> receivedPacket->adminName;
        packet >> receivedPacket->isPrivate;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::RESPONSE_CREATE_LOBBY:
    {
        auto receivedPacket = std::make_unique<CreateLobbyResponsePacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->success;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::JOIN_LOBBY:
    {
        auto receivedPacket = std::make_unique<JoinLobbyPacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->lobbyId;
        packet >> receivedPacket->login;
        packet >> receivedPacket->password;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::RESPONSE_JOIN_LOBBY:
    {
        auto receivedPacket = std::make_unique<JoinLobbyResponsePacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->response;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::UPDATE_LOBBY:
    {
        auto receivedPacket = std::make_unique<LobbyUpdatePacket>();
        receivedPacket->header = header;
        packet >> receivedPacket->lobbyData.lobbyName;
        packet >> receivedPacket->lobbyData.lobbyId;
        packet >> receivedPacket->lobbyData.playersAmount;
        packet >> receivedPacket->lobbyData.maxNumberOfPlayers;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::LEAVE_LOBBY:
    {
        auto receivedPacket = std::make_unique<NetworkPacket>();
        receivedPacket->header = header;
        sendPacket(std::move(receivedPacket));
        break;
    }
    case PacketType::INSIDE_LOBBY_DATA_UPDATE:
    {
        std::cout << "Inside lobby data\n";
        auto receviedPacket = std::make_unique<InsideLobbyDataUpdatePacket>();
        receviedPacket->header = header;
        
        packet >> receviedPacket->playersSize;

        std::vector<InsideLobbyDataUpdatePacket::Player> players;
        players.reserve(receviedPacket->playersSize);

        for(int index = 0; index < receviedPacket->playersSize; ++index)
        {
            InsideLobbyDataUpdatePacket::Player player;
            packet >> player.name;
            players.push_back(player);
        }

        receviedPacket->players = players;

        sendPacket(std::move(receviedPacket));
        break;
    }
    default:
    {
        std::cerr << "Broken packet\n";
        return;
    }
    }
}

void PacketParser::sendPacket(std::unique_ptr<NetworkPacket> packet)
{
    if (!m_onPacketParsed)
        return;

    PacketType packetType = static_cast<PacketType>(packet->header.type);

    m_onPacketParsed(std::move(packet), packetType);
}

void PacketParser::setOnParsedPacket(const PacketParser::PacketReceivedFunction &function)
{
    m_onPacketParsed = function;
}

// TCP
//  void processTcpBuffer()
//  {
//      while (true)
//      {
//          // Need at least 4 bytes to read header
//          if (m_tcpBuffer.size() < sizeof(NetworkPacketHeader))
//              return;

//         // Read header safely
//         NetworkPacketHeader header;
//         std::memcpy(&header, m_tcpBuffer.data(), sizeof(header));

//         // Convert from network byte order if needed
//         uint16_t type = header.type;
//         uint16_t size = header.size;

//         // Check if full payload arrived
//         size_t totalPacketSize = sizeof(NetworkPacketHeader) + size;

//         if (m_tcpBuffer.size() < totalPacketSize)
//         {
//             // Not enough data yet, wait for next receive()
//             return;
//         }

//         // We have a full packet! Extract it.
//         std::vector<uint8_t> fullPacket(
//             m_tcpBuffer.begin(),
//             m_tcpBuffer.begin() + totalPacketSize
//         );

//         // Remove from buffer
//         m_tcpBuffer.erase(
//             m_tcpBuffer.begin(),
//             m_tcpBuffer.begin() + totalPacketSize
//         );

//         // Convert raw bytes into an SFML packet for your parser
//         sf::Packet sfPacket;
//         sfPacket.append(fullPacket.data() + sizeof(NetworkPacketHeader),
//                         size);

//         // Parse the packet
//         m_parser.parsePacket(sfPacket);
//     }
// }
