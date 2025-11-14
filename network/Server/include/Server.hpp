#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include <SFML/Network.hpp>


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

    sf::UdpSocket m_udpSocket;
    sf::TcpListener m_tcpListener;
    bool m_isServerRunning{false};
};

#endif //NET_SERVER_HPP