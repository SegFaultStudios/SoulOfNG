#include "Server.hpp"
#include <csignal> 
#include <iostream>
#include "Packet.hpp"

volatile sig_atomic_t g_ctrl_c_pressed = 0;

Server::Server()
{
    m_lobbies.push_back(Lobby{
        .playersAmount = 1,
        .lobbyName = "Something",
    });
}

void Server::start()
{
    sf::IpAddress ip = sf::IpAddress::Any;
    unsigned short udpPort = 8080;
    unsigned short tcpPort = 9090;

    if(sf::Socket::Status status = m_udpSocket.bind(udpPort, ip); status != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind UDP socket");

    if(sf::Socket::Status status = m_tcpListener.listen(tcpPort, ip); status != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind TCP socket");

    m_tcpListener.setBlocking(false);
    m_udpSocket.setBlocking(false);

    std::cout << "Successfully started server" << std::endl;


    m_isServerRunning = true;

    run();
}

void Server::stop()
{
    m_isServerRunning = false;
}

void Server::run()
{
    while(m_isServerRunning)
    {
        if(g_ctrl_c_pressed)
        {
            stop();
            break;
        }

        std::optional<sf::IpAddress> sender{sf::IpAddress::Any};
        sf::Packet packet;

        unsigned short port;

        if (m_udpSocket.receive(packet, sender, port) == sf::Socket::Status::Done)
        {
            PacketHeader header;
            uint16_t packetType = static_cast<uint16_t>(header.type);

            packet >> packetType;
            packet >> header.size;

            header.type = static_cast<PacketType>(packetType);

            int32_t timestamp;

            packet >> timestamp;

            // std::cout << "Received " << type << " " << timestamp << std::endl;

            if (header.type == PacketType::PING)
            {
                sf::Packet reply;
                reply << int32_t(PacketType::PONG) << timestamp;
                sf::Socket::Status status = m_udpSocket.send(reply, sender.value(), port);
            }
            else
                std::cerr << "Unknown packet type: " << packetType << std::endl;
        }

        std::unique_ptr<sf::TcpSocket> clientSocket = std::make_unique<sf::TcpSocket>(); 

        if(m_tcpListener.accept(*clientSocket.get()) == sf::Socket::Status::Done)
        {
            std::cout << "New client" << std::endl;
            clientSocket->setBlocking(false);
            m_clients.push_back(std::move(clientSocket));
        }

        handleClients();
    }

    std::cerr << "Server stoped" << std::endl;
}

void Server::handleClients()
{
    for(int clientIndex = 0; clientIndex < m_clients.size(); ++clientIndex)
    {
        auto& client = m_clients.at(clientIndex);

        sf::Packet packet;

        const sf::Socket::Status status = client->receive(packet);

        if(status == sf::Socket::Status::Done)
        {
            int32_t type;

            packet >> type;

            if(type == static_cast<int32_t>(PacketType::ASK_FOR_LOBBIES))
            {
                std::cout << "Asking for lobbies" << std::endl;

                Packet lobbiesPacket(PacketType::GET_LOBBIES);
                lobbiesPacket.write(static_cast<uint16_t>(m_lobbies.size()));

                for(const auto& lobby : m_lobbies)
                {
                    lobbiesPacket << lobby.lobbyName;
                }

                auto sendPacket = lobbiesPacket.finalize();

                auto sendStatus = client->send(sendPacket);
            }
        }
        else if(status == sf::Socket::Status::Disconnected)
        {
            client.reset();
            m_clients.erase(m_clients.cbegin() + clientIndex);
            std::cout << "Client disconnected" << std::endl;
        }
    }
}

Server::~Server()
{

}