#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include <SFML/Network.hpp>

#include "Types.hpp"

#include <vector>
#include <string>

struct Lobby
{
    uint16_t numberOfPlayers{0};
    uint16_t maxNumberOfPlayers{4};
    std::string lobbyName;
};

//TODO add IMGUI SERVER(BUILD_SERVER_GUI FLAG IN CMAKE)
class Server
{
public:
    Server();

    void start();

    void stop();

    ~Server();
private:
    void run();

    void handleClients();

    sf::UdpSocket m_udpSocket;
    sf::TcpListener m_tcpListener;
    bool m_isServerRunning{false};

    std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;

    std::vector<Lobby> m_lobbies;
};

#endif //NET_SERVER_HPP