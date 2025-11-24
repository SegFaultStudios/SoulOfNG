#include "Server/Server.hpp"
#include <csignal>
#include <iostream>
#include "Packet.hpp"

Server::Server()
{
    m_parser.setOnParsedPacket(std::bind(&Server::onParsedMessage, this, std::placeholders::_1, std::placeholders::_2));

    m_clientConnections.reserve(128);
}

void Server::onParsedMessage(std::unique_ptr<NetworkPacket> packet, PacketType packetType)
{
    if (packetType == PacketType::PING)
    {
        PingPongPacket *receivedPacket = dynamic_cast<PingPongPacket*>(packet.get());

        Packet reply(PacketType::PONG);
        reply.write(receivedPacket->timestamp);
        auto replyPacket = reply.finalize();
        sf::Socket::Status status = m_udpSocket.send(replyPacket, m_lastIpAddress, m_lastPort);
    }
    else if (packetType == PacketType::GET_LOBBIES)
    {
        if (!m_lastClient)
            return;

        Packet lobbiesPacket(PacketType::RESPONSE_GET_LOBBIES);
        lobbiesPacket.write(static_cast<uint16_t>(m_lobbies.size()));

        for (const auto &lobby : m_lobbies)
        {
            lobbiesPacket << lobby.lobbyName;
            lobbiesPacket << lobby.lobbyId;
            lobbiesPacket << lobby.numberOfPlayers;
            lobbiesPacket << lobby.maxNumberOfPlayers;
        }

        auto sendPacket = lobbiesPacket.finalize();

        auto sendStatus = m_lastClient->send(sendPacket);
    }
    else if(packetType == PacketType::CREATE_LOBBY)
    {
        bool responseValue{true};

        CreateLobbyPacket* receivedPacket = dynamic_cast<CreateLobbyPacket*>(packet.get());

        auto client = getClientConnectionFromSocket(m_lastClient);

        if(!client)
        {
            responseValue = false;
            std::cerr << "Failed to find a client connection\n";
            Packet response(PacketType::RESPONSE_CREATE_LOBBY);
            response.write(responseValue);
            sendMessageToClient(m_lastClient, response.finalize());
            return;
        }

        Lobby newLobby;
        newLobby.numberOfPlayers = 0,
        newLobby.maxNumberOfPlayers = receivedPacket->maxNumberOfPlayers,
        newLobby.lobbyId = m_nextLobbyId,
        newLobby.lobbyName = receivedPacket->lobbyName;

        std::string clientName = receivedPacket->adminName;

        client->lobbyId = newLobby.lobbyId;
        client->setName(clientName);
        client->socket = m_lastClient;

        newLobby.addClient(client);
        m_lobbies.push_back(std::move(newLobby));
        ++m_nextLobbyId;

        Lobby& lobby = m_lobbies.back();

        Packet response(PacketType::RESPONSE_CREATE_LOBBY);
        response.write(responseValue);
        sendMessageToClient(m_lastClient, response.finalize());

        Packet updateLobbyPacket(PacketType::INSIDE_LOBBY_DATA_UPDATE);
        updateLobbyPacket.write(lobby.numberOfPlayers);
        updateLobbyPacket.write(client->getName());
        sendMessageToClient(m_lastClient, updateLobbyPacket.finalize());

        // for(const auto& player : lobby.getPlayers())
        // {
        //     updateLobbyPacket.write(player->getName());
        // }

        // for(const auto& player : lobby.getPlayers())
        //     sendMessageToClient(player->socket, updateLobbyPacket.finalize());

    }
    else if(packetType == PacketType::JOIN_LOBBY)
    {
        std::cout << "Trying to join\n";
        JoinLobbyPacket* receivedPacket = dynamic_cast<JoinLobbyPacket*>(packet.get());

        auto it = std::find_if(m_lobbies.begin(), m_lobbies.end(), [&receivedPacket](const Lobby& lobby)
        {
            return lobby.lobbyId == receivedPacket->lobbyId;
        });

        Packet response(PacketType::RESPONSE_JOIN_LOBBY);
        uint8_t responseType(JoinLobbyResponsePacket::JoinLobbyResponseType::SUCCESS);

        if (it == m_lobbies.end())
        {
            responseType = JoinLobbyResponsePacket::JoinLobbyResponseType::FAILED;
            response.write(responseType);
            sendMessageToClient(m_lastClient, response.finalize());
            return;
        }

        Lobby& lobby = *it;

        if (lobby.isFull())
        {
            responseType = JoinLobbyResponsePacket::JoinLobbyResponseType::FAILED;
            response.write(responseType);
            sendMessageToClient(m_lastClient, response.finalize());
            return;
        }

        ClientConnection* client = getClientConnectionFromSocket(m_lastClient);

        if(!client)
        {
            std::cerr << "Failed to get client connection from socket\n";
            responseType = JoinLobbyResponsePacket::JoinLobbyResponseType::FAILED;
            response.write(responseType);
            sendMessageToClient(m_lastClient, response.finalize());
            return;
        }

        client->lobbyId = lobby.lobbyId;
        client->setName(receivedPacket->login);
        client->socket = m_lastClient;

        lobby.addClient(client);

        response.write(responseType);
        std::cout << "Success response\n";
        sendMessageToClient(m_lastClient, response.finalize());

        Packet packet(PacketType::UPDATE_LOBBY);
        packet.write(lobby.lobbyName);
        packet.write(lobby.lobbyId);
        packet.write(lobby.numberOfPlayers);
        packet.write(lobby.maxNumberOfPlayers);
        sendMessageToAllClients(packet.finalize());

        Packet updateLobbyPacket(PacketType::INSIDE_LOBBY_DATA_UPDATE);
        updateLobbyPacket.write(lobby.numberOfPlayers);

        for(const auto& player : lobby.getPlayers())
            updateLobbyPacket.write(player->getName());

        sf::Packet payload = updateLobbyPacket.finalize();
        for(const auto& player : lobby.getPlayers())
        {
            if(!player->socket)
            {
                std::cerr << "No player socket\n";
                continue;
            }
            
            sendMessageToClient(player->socket, payload);
        }

        std::cout << "Client successfully joined the lobby\n";
    }
    else if(packetType == PacketType::LEAVE_LOBBY)
    {
        std::cout << "Leaving the lobby\n";

        auto clientConnection = getClientConnectionFromSocket(m_lastClient);

        auto lobbyIt = std::find_if(m_lobbies.begin(), m_lobbies.end(), [&clientConnection](Lobby& lobby)
        {
            return clientConnection->lobbyId == lobby.lobbyId;
        });

        if(lobbyIt != m_lobbies.end())
        {
            if(!lobbyIt->removeClient(clientConnection))
            {
                std::cerr << "Failed to remove player from lobby (Lobby name: " << lobbyIt->lobbyName << "(" << lobbyIt->lobbyId << ")\n";
            }

            clientConnection->lobbyId = UINT32_MAX;

            Packet updateLobbyPacket(PacketType::INSIDE_LOBBY_DATA_UPDATE);
            updateLobbyPacket.write(lobbyIt->numberOfPlayers);

            for(const auto& player : lobbyIt->getPlayers())
                updateLobbyPacket.write(player->getName());

            for(const auto& player : lobbyIt->getPlayers())
                sendMessageToClient(player->socket, updateLobbyPacket.finalize());
        }
    }
}

ClientConnection* Server::getClientConnectionFromSocket(sf::TcpSocket* socket)
{
    if(!socket)
        return nullptr;
    
    auto it = std::find_if(m_clientConnections.begin(), m_clientConnections.end(), [&socket](auto& client)
    {
        return client.socket == socket;
    });

    return it == m_clientConnections.end() ? nullptr : &(*it);
}

void Server::start()
{
    sf::IpAddress ip = sf::IpAddress::Any;
    unsigned short udpPort = 8080;
    unsigned short tcpPort = 9090;

    if (sf::Socket::Status status = m_udpSocket.bind(udpPort, ip); status != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind UDP socket");

    if (sf::Socket::Status status = m_tcpListener.listen(tcpPort, ip); status != sf::Socket::Status::Done)
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
    while (m_isServerRunning)
    {
        std::optional<sf::IpAddress> sender{sf::IpAddress::Any};
        sf::Packet packet;

        if (m_udpSocket.receive(packet, sender, m_lastPort) == sf::Socket::Status::Done)
        {
            m_lastIpAddress = sender.value();

            m_parser.parsePacket(packet);
        }

        // Memory is fucked...
        auto clientSocket = std::make_unique<sf::TcpSocket>();

        if (m_tcpListener.accept(*clientSocket.get()) == sf::Socket::Status::Done)
            connectClient(std::move(clientSocket));

        handleClients();
    }

    std::cerr << "Server stoped" << std::endl;
}

void Server::connectClient(std::unique_ptr<sf::TcpSocket> client)
{
    std::cout << "New client" << std::endl;
    client->setBlocking(false);
    ClientConnection clientConnection;
    clientConnection.socket = client.get();
    clientConnection.playerId = m_nextPlayerId;
    ++m_nextPlayerId;

    m_clients.push_back(std::move(client));
    m_clientConnections.push_back(clientConnection);
}

void Server::handleClients()
{
    for (int clientIndex = 0; clientIndex < m_clients.size(); ++clientIndex)
    {
        auto &client = m_clients[clientIndex];

        sf::Packet packet;

        const sf::Socket::Status status = client->receive(packet);

        if (status == sf::Socket::Status::Done)
        {
            m_lastClient = client.get();
            m_parser.parsePacket(packet);
        }
        else if (status == sf::Socket::Status::Disconnected)
        {
            disconnectClient(client.get());
            client.reset();
            m_clients.erase(m_clients.cbegin() + clientIndex);
            --clientIndex;
            std::cout << "Client disconnected\n";
        }
    }
}

void Server::disconnectClient(sf::TcpSocket* client)
{
    auto it = std::find_if(m_clientConnections.begin(), m_clientConnections.end(), [&client](ClientConnection& clientConnection)
    {
        return clientConnection.socket == client;
    }); 

    if(it == m_clientConnections.end())
    {
        std::cerr << "ERROR: FAILED TO FIND CLIENT CONNECTION WHILE DISCONNECTING CLIENT. SOMETHING IS WRONG\n";
        return;
    }

    ClientConnection& clientConnection = *it;

    if(clientConnection.lobbyId != UINT32_MAX)
    {
        auto lobbyIt = std::find_if(m_lobbies.begin(), m_lobbies.end(), [&clientConnection](Lobby& lobby)
        {
            return clientConnection.lobbyId == lobby.lobbyId;
        });

        if(lobbyIt != m_lobbies.end())
        {
            if(!lobbyIt->removeClient(&clientConnection))
            {
                std::cerr << "Failed to remove player from lobby (Lobby name: " << lobbyIt->lobbyName << "(" << lobbyIt->lobbyId << ")\n";
            }

            clientConnection.lobbyId = UINT32_MAX;
            
            Packet packet(PacketType::UPDATE_LOBBY);
            packet.write(lobbyIt->lobbyName);
            packet.write(lobbyIt->lobbyId);
            packet.write(lobbyIt->numberOfPlayers);
            packet.write(lobbyIt->maxNumberOfPlayers);
            sendMessageToAllClients(packet.finalize());
        }

        m_clientConnections.erase(it);
    }
}

bool Server::sendMessageToAllClients(sf::Packet& packet)
{
    bool result{true};

    for(const auto& client : m_clients)
        if(!sendMessageToClient(client.get(), packet))
            result = false;

    return result;
}

bool Server::sendMessageToClient(sf::TcpSocket* client, sf::Packet& packet)
{
    sf::TcpSocket::Status status = client->send(packet);

    if(status != sf::TcpSocket::Status::Done)
    {
        std::cerr << "Got an error while sending message to a client\n";
        return false;
    }

    return true;
}

Server::~Server()
{
}