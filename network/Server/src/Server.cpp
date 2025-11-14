#include "Server.hpp"
#include <csignal> 
#include <iostream>

volatile sig_atomic_t g_ctrl_c_pressed = 0;

Server::Server()
{

}

void Server::start()
{
    sf::IpAddress ip = sf::IpAddress::LocalHost;
    unsigned short udpPort = 8080;
    unsigned short tcpPort = 9090;

    if(sf::Socket::Status status = m_udpSocket.bind(udpPort, ip); status != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind UDP socket");

    if(sf::Socket::Status status = m_tcpListener.listen(tcpPort, ip); status != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind TCP socket");

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

        std::array<char, 100> data;
        std::size_t received;
        std::optional<sf::IpAddress> sender{sf::IpAddress::Any};
        
        unsigned short port;

        if (m_udpSocket.receive(data.data(), data.size(), received, sender, port) == sf::Socket::Status::Done)
        {
            // std::cout << "Received from " << senderIp.toString() << ":" << senderPort << ": " << buffer << std::endl;
        }
    }

    std::cerr << "Server stoped" << std::endl;
}

Server::~Server()
{

}