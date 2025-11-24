#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <SFML/Network.hpp>

#include <cstdint>

class ClientConnection
{
public:
    sf::TcpSocket* socket{nullptr};
    sf::IpAddress ip{sf::IpAddress::Any};

    unsigned short udpPort;

    uint32_t playerId;
    uint32_t lobbyId{UINT32_MAX};

    void setName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& getName() const
    {
        return m_name;
    }
private:
    std::string m_name;
};

#endif //CLIENT_CONNECTION_HPP