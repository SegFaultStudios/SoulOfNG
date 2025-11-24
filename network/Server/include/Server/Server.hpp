#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include <SFML/Network.hpp>

#include "Server/Lobby.hpp"
#include "Server/ClientConnection.hpp"

#include "Types.hpp"
#include "PacketParser.hpp"

#include <vector>
#include <string>

// TODO add IMGUI SERVER(BUILD_SERVER_GUI FLAG IN CMAKE)
class Server
{
public:
    Server();

    void start();

    void stop();

    ~Server();

private:
    void onParsedMessage(std::unique_ptr<NetworkPacket> packet, PacketType packetType);

    ClientConnection* getClientConnectionFromSocket(sf::TcpSocket* socket);

    bool sendMessageToAllClients(sf::Packet& packet);
    bool sendMessageToClient(sf::TcpSocket* client, sf::Packet& packet);

    void run();

    PacketParser m_parser;

    void handleClients();

    void disconnectClient(sf::TcpSocket* client);
    void connectClient(std::unique_ptr<sf::TcpSocket> client);

    sf::UdpSocket m_udpSocket;
    sf::TcpListener m_tcpListener;
    bool m_isServerRunning{false};

    std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;

    // std::vector<std::unique_ptr<ClientConnection>> m_clientConnections;
    std::vector<ClientConnection> m_clientConnections;

    std::vector<Lobby> m_lobbies;

    uint32_t m_nextPlayerId{0};
    uint32_t m_nextLobbyId{0};

    sf::IpAddress m_lastIpAddress{sf::IpAddress::Any};
    unsigned short m_lastPort;
    sf::TcpSocket* m_lastClient{nullptr};
};

#endif // NET_SERVER_HPP