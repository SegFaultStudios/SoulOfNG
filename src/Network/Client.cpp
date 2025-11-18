#include "Network/Client.hpp"
#include <fstream>

#include <iostream>
#include <nlohmann/json.hpp>

#include "Packet.hpp"

Client::Client()
{
    tryToReadConfigFile();

    m_pingTimer = std::make_unique<Timer>();
    m_pingTimeoutTimer = std::make_unique<Timer>();

    m_parser.setOnParsedPacket(std::bind(&Client::onParsedMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::onParsedMessage(std::unique_ptr<NetworkPacket> packet, PacketType packetType)
{
    if(packetType == PacketType::PONG)
    {
        PingPongPacket* receivedPacket = dynamic_cast<PingPongPacket*>(packet.get());

        if(!receivedPacket)
        {
            std::cerr << "Broked PING packet\n";
            return;
        }

        m_ping = m_clock.getElapsedTime().asMilliseconds() - receivedPacket->timestamp;
        std::cout << "Ping: " << m_ping << " ms\n";
        m_pingTimeoutTimer->restart();
    }
}

void Client::tryToReadConfigFile()
{
    const std::string configPath = "./resources/config/client_config.json";

    std::ifstream file(configPath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " <<  configPath << std::endl;
        return;
    }

    nlohmann::json json;

    try
    {
        file >> json;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "Failed to parse config file " << e.what() << std::endl;
        return;
    }

    std::string udpIp = json.value("server_udp_ip", "127.0.0.1");
    unsigned short udpPort = json.value("server_udp_port", 8080);
    std::string tcpIp = json.value("server_tcp_ip", "127.0.0.1");
    unsigned short tcpPort = json.value("server_tcp_port", 9090);

    m_udpConnectionData.ipAddress = sf::IpAddress::resolve(udpIp).value();
    m_udpConnectionData.port = udpPort;
    m_tcpConnectionData.ipAddress = sf::IpAddress::resolve(tcpIp).value();
    m_tcpConnectionData.port = tcpPort;
}

void Client::connect()
{
    if(m_isTryingToConnect || m_isConnected)
        return;

    m_isTryingToConnect = true;

    m_connectionFuture = std::async([this]{return tryToConnect();});
}

void Client::stop()
{
    m_isStopped = true;
    m_pingTimer->stop();

    if(m_isTryingToConnect)
    {
        if(m_connectionFuture.valid())
        {
            m_connectionFuture.wait(); //Kinda bad
        }
    }

    m_isTryingToConnect = false;
}

void Client::update()
{
    m_pingTimer->update();
    m_pingTimeoutTimer->update();

    if(m_isTryingToConnect)
    {
        if(m_connectionFuture.valid())
        {
            if(m_connectionFuture.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
            {
                m_isTryingToConnect = false;

                m_isConnected = m_connectionFuture.get();

                m_isConnected ? connected.emit() : failedToConnect.emit();

                if(m_isConnected)
                {
                    m_udpSocket.setBlocking(false);
                    m_tcpSocket.setBlocking(false);
                }
            }
        }

        return;
    }

    if(m_isConnected)
    {
        checkUdpSocket();
        checkTcpSocket();
    }
}

void Client::checkUdpSocket()
{
    if(!m_isConnected)
        return;

    sf::Packet packet;
    std::optional<sf::IpAddress> sender;
    unsigned short port;

    sf::Socket::Status status = m_udpSocket.receive(packet, sender, port);

    if (status == sf::Socket::Status::Done)
    {
        m_parser.parsePacket(packet);
        // uint16_t type;
        // uint16_t size;

        // int32_t timestamp;

        // packet >> type >> size >> timestamp;

        // if (type == static_cast<int32_t>(PacketType::PONG))
        // {
        //     m_ping = m_clock.getElapsedTime().asMilliseconds() - timestamp;
        //     std::cout << "Ping: " << m_ping << " ms\n";
        //     m_pingTimeoutTimer->restart();
        // }
        // else
        //     receivedUDPData.emit(packet);
    }
    else if(status == sf::Socket::Status::Disconnected)
    {
        std::cerr << "Failed to reach the server. Disconnecting..." << std::endl;
        return disconnect();
    }
    else
    {
    }
}

void Client::disconnect()
{
    if(!m_isConnected)
        return;
    
    m_isConnected = false;
    m_pingTimer->stop();
}

void Client::checkTcpSocket()
{
    if(!m_isConnected)
        return;

    sf::Packet packet;

    if(m_tcpSocket.receive(packet) == sf::Socket::Status::Done)
    {
        PacketHeader header;
        uint16_t packetType = static_cast<uint16_t>(header.type);

        packet >> packetType;
        packet >> header.size;

        header.type = static_cast<PacketType>(packetType);

        if(header.type == PacketType::GET_LOBBIES)
        {
            std::cout << "Got lobbies" << std::endl;

            uint16_t lobbiesSize;

            packet >> lobbiesSize;

            std::vector<LobbyData> lobbies;
            lobbies.reserve(lobbiesSize);

            for(int index = 0; index < lobbiesSize; ++index)
            {
                std::string lobbyName;
                uint16_t playersAmount;
                uint16_t maxNumberOfPlayers;

                packet >> lobbyName;
                packet >> playersAmount;
                packet >> maxNumberOfPlayers;

                std::cout << "Lobby name: " << lobbyName  << " " << playersAmount << "/" << maxNumberOfPlayers << std::endl;

                LobbyData lobbyData
                {
                    .name = lobbyName,
                    .numberOfPlayers = playersAmount,
                    .maxNumberOfPlayers = maxNumberOfPlayers,
                };

                lobbies.push_back(lobbyData);
            }

            receivedLobbies.emit(lobbies);
        }

        receivedTCPData.emit(packet);
    }
}

void Client::startPing()
{
    m_pingTimer->start(1);
    m_pingTimeoutTimer->start(5);

    HANDLE_EVENT(m_pingTimeoutTimer, Timer::timeout, this, [this]
    {
        std::cerr << "Server does not response. Disconnecting..." << std::endl;
        disconnect();
    });

    HANDLE_EVENT(m_pingTimer, Timer::timeout, this, [this] 
    {
        sendPing();
    });
}

void Client::askForLobbies()
{
    sf::Packet packet;
    packet << int32_t(PacketType::ASK_FOR_LOBBIES);

    sf::Socket::Status status = m_tcpSocket.send(packet);
}

bool Client::isConnected() const
{
    return m_isConnected;
}

void Client::sendPing()
{
    m_lastPingTime = m_clock.getElapsedTime().asMilliseconds();

    Packet packet(PacketType::PING);
    packet << m_lastPingTime;
    auto sendPacket = packet.finalize();

    sf::Socket::Status status = m_udpSocket.send(sendPacket, m_udpConnectionData.ipAddress, m_udpConnectionData.port);
}

bool Client::tryToConnect()
{
    int tries = 0;

    static constexpr unsigned short MAX_TRIES = 5;

    while(tries < MAX_TRIES && !m_isStopped)
    {
        sf::Socket::Status connectionStatus = m_tcpSocket.connect(m_tcpConnectionData.ipAddress, m_tcpConnectionData.port);

        if(connectionStatus == sf::Socket::Status::Done)
            return true;

        ++tries;

        //SFML timeout does not work for some reason
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return false;
}