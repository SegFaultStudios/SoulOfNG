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
    if (packetType == PacketType::PONG)
    {
        PingPongPacket *receivedPacket = dynamic_cast<PingPongPacket *>(packet.get());

        if (!receivedPacket)
        {
            std::cerr << "Broked PING packet\n";
            return;
        }

        m_ping = m_clock.getElapsedTime().asMilliseconds() - receivedPacket->timestamp;
        // std::cout << "Ping: " << m_ping << " ms\n";
        m_pingTimeoutTimer->restart();
    }
    else if (packetType == PacketType::RESPONSE_GET_LOBBIES)
    {
        GetLobbiesPacket *receivedPacket = dynamic_cast<GetLobbiesPacket*>(packet.get());

        if (!receivedPacket)
        {
            std::cerr << "Broken lobbies packet\n";
            return;
        }

        std::vector<LobbyData> lobbies;
        lobbies.reserve(receivedPacket->lobbies.size());

        for (const auto &lobby : receivedPacket->lobbies)
        {
            LobbyData lobbyData;
            lobbyData.lobbyId = lobby.lobbyId;
            lobbyData.name = lobby.lobbyName;
            lobbyData.maxNumberOfPlayers = lobby.maxNumberOfPlayers;
            lobbyData.numberOfPlayers = lobby.playersAmount;

            lobbies.push_back(lobbyData);
        }

        receivedLobbies.emit(lobbies);
    }
    else if(packetType == PacketType::RESPONSE_CREATE_LOBBY)
    {
        CreateLobbyResponsePacket *receivedPacket = dynamic_cast<CreateLobbyResponsePacket*>(packet.get());

        lobbyCreated.emit(receivedPacket->success);
    }
    else if(packetType == PacketType::RESPONSE_JOIN_LOBBY)
    {
        JoinLobbyResponsePacket* receivedPacket = dynamic_cast<JoinLobbyResponsePacket*>(packet.get());

        joinLobbyAnswer.emit(receivedPacket->response);
    }
    else if(packetType == PacketType::UPDATE_LOBBY)
    {
        auto* receivedPacket = dynamic_cast<LobbyUpdatePacket*>(packet.get());

        if(!receivedPacket)
        {
            std::cerr << "Broken lobby update packet\n";
            return;
        }

        LobbyData lobbyData;
        lobbyData.name = receivedPacket->lobbyData.lobbyName;
        lobbyData.lobbyId = receivedPacket->lobbyData.lobbyId;
        lobbyData.numberOfPlayers = receivedPacket->lobbyData.playersAmount;
        lobbyData.maxNumberOfPlayers = receivedPacket->lobbyData.maxNumberOfPlayers;

        lobbyUpdate.emit(lobbyData);
    }
    else if(packetType == PacketType::INSIDE_LOBBY_DATA_UPDATE)
    {
        auto* receivedPacket = dynamic_cast<InsideLobbyDataUpdatePacket*>(packet.get());

        if(!receivedPacket)
        {
            std::cerr << "Broken inside lobby data packet\n";
            return;
        }

        std::vector<std::string> playerNames;

        playerNames.reserve(receivedPacket->playersSize);

        for(const auto& player : receivedPacket->players)
            playerNames.push_back(player.name);

        lobbyPlayersData.emit(playerNames);
    }
}

void Client::tryToReadConfigFile()
{
    const std::string configPath = "./resources/config/client_config.json";

    std::ifstream file(configPath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << configPath << std::endl;
        return;
    }

    nlohmann::json json;

    try
    {
        file >> json;
    }
    catch (const nlohmann::json::parse_error &e)
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
    if (m_isTryingToConnect || m_isConnected)
        return;

    m_isTryingToConnect = true;

    m_connectionFuture = std::async([this]
                                    { return tryToConnect(); });
}

void Client::stop()
{
    m_isStopped = true;
    m_pingTimer->stop();

    if (m_isTryingToConnect)
    {
        if (m_connectionFuture.valid())
        {
            m_connectionFuture.wait(); // Kinda bad
        }
    }

    m_isTryingToConnect = false;
}

void Client::update()
{
    m_pingTimer->update();
    m_pingTimeoutTimer->update();

    if (m_isTryingToConnect)
    {
        if (m_connectionFuture.valid())
        {
            if (m_connectionFuture.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
            {
                m_isTryingToConnect = false;

                m_isConnected = m_connectionFuture.get();

                m_isConnected ? connected.emit() : failedToConnect.emit();

                if (m_isConnected)
                {
                    m_udpSocket.setBlocking(false);
                    m_tcpSocket.setBlocking(false);
                }
            }
        }

        return;
    }

    if (m_isConnected)
    {
        checkUdpSocket();
        checkTcpSocket();
    }
}

void Client::checkUdpSocket()
{
    if (!m_isConnected)
        return;

    sf::Packet packet;
    std::optional<sf::IpAddress> sender;
    unsigned short port;

    sf::Socket::Status status = m_udpSocket.receive(packet, sender, port);

    if (status == sf::Socket::Status::Done)
        return m_parser.parsePacket(packet);

    if (status == sf::Socket::Status::Disconnected)
    {
        std::cerr << "Failed to reach the server. Disconnecting..." << std::endl;
        return disconnect();
    }
    // else if (status == sf::Socket::Status::Error)
    // {
    //     std::cerr << "Error while receiving UDP traffic\n";
    //     return;
    // }
    else
    {
    }
}

void Client::authorization(const std::string login)
{
    if(m_isAuthorized)
        return;
    
    m_isAuthorized = true;
    m_login = login;

    std::cout << "Client is authorized with " << login << " name\n";
}

bool Client::isAuthorized() const
{
    return m_isAuthorized;
}

void Client::joinLobby(uint32_t lobbyId)
{
    Packet packet(PacketType::JOIN_LOBBY);

    std::string password{""};

    packet.write(lobbyId);
    packet.write(m_login);
    packet.write(password);

    auto sfPacket = packet.finalize();

    auto sendResult = m_tcpSocket.send(sfPacket);
}

void Client::askToCreateLobby(const LobbyData& lobbyData)
{
    Packet packet(PacketType::CREATE_LOBBY);

    packet.write(lobbyData.name);
    packet.write(lobbyData.maxNumberOfPlayers);
    packet.write(m_login);
    packet.write(true);

    auto sfPacket = packet.finalize();

    m_tcpSocket.send(sfPacket);
}

void Client::disconnect()
{
    if (!m_isConnected)
        return;

    m_isConnected = false;
    m_pingTimer->stop();
}

void Client::checkTcpSocket()
{
    if (!m_isConnected)
        return;

    sf::Packet packet;

    auto status = m_tcpSocket.receive(packet);

    if (status == sf::Socket::Status::Done)
        m_parser.parsePacket(packet);
    else if(status == sf::Socket::Status::NotReady)
        std::cerr << "Data not ready\n";
    else if(status == sf::Socket::Status::Error)
        std::cerr << "Data is error\n";
    else
        std::cerr << "Something happened while receiving TCP data\n";
    
}

void Client::leaveLobby()
{
    Packet packet(PacketType::LEAVE_LOBBY);
    auto sfPacket = packet.finalize();
    sf::Socket::Status status = m_tcpSocket.send(sfPacket);
}

void Client::startPing()
{
    m_pingTimer->start(1);
    m_pingTimeoutTimer->start(5);

    HANDLE_EVENT(m_pingTimeoutTimer, Timer::timeout, this, [this]
                 {
        std::cerr << "Server does not response. Disconnecting..." << std::endl;
        disconnect(); });

    HANDLE_EVENT(m_pingTimer, Timer::timeout, this, [this]
                 { sendPing(); });
}

void Client::askForLobbies()
{
    Packet packet(PacketType::GET_LOBBIES);
    auto sfPacket = packet.finalize();

    sf::Socket::Status status = m_tcpSocket.send(sfPacket);
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

    while (tries < MAX_TRIES && !m_isStopped)
    {
        sf::Socket::Status connectionStatus = m_tcpSocket.connect(m_tcpConnectionData.ipAddress, m_tcpConnectionData.port);

        if (connectionStatus == sf::Socket::Status::Done)
            return true;

        ++tries;

        // SFML timeout does not work for some reason
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return false;
}