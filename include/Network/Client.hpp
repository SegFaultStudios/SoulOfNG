#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Network.hpp>

#include "Signal.hpp"
#include "Types.hpp"
#include "Timer.hpp"
#include "Lobby.hpp"
#include "PacketParser.hpp"

#include <future>
#include <memory>

class Client
{
public:
    using UniquePtr = std::unique_ptr<Client>;

    Signal<> connected;
    Signal<> disconnected;
    Signal<> failedToConnect;
    Signal<> lostConnection;
    Signal<sf::Packet> receivedUDPData;
    Signal<sf::Packet> receivedTCPData;

    Signal<const std::vector<LobbyData>&> receivedLobbies;

    Client();

    void update();

    void connect();

    void disconnect();

    void stop();

    void startPing();

    void askForLobbies();

    bool isConnected() const;
private:
    void checkUdpSocket();
    void checkTcpSocket();

    void onParsedMessage(std::unique_ptr<NetworkPacket> packet, PacketType packetType);

    void tryToReadConfigFile();
    
    bool tryToConnect();

    void sendPing();

    bool m_isConnected{false};

    int32_t m_ping{-1};
    sf::Clock m_clock;
    int32_t m_lastPingTime{0};

    struct ConnectionData
    {
        sf::IpAddress ipAddress{sf::IpAddress::LocalHost};
        unsigned short port{0000};
    };

    PacketParser m_parser;

    ConnectionData m_udpConnectionData;
    ConnectionData m_tcpConnectionData;

    sf::TcpSocket m_tcpSocket;
    sf::UdpSocket m_udpSocket;

    bool m_isTryingToConnect{false};
    bool m_isStopped{false};

    std::future<bool> m_connectionFuture;

    std::unique_ptr<Timer> m_pingTimer{nullptr};
    std::unique_ptr<Timer> m_pingTimeoutTimer{nullptr};
};

#endif //CLIENT_HPP