#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Network.hpp>

#include "Signal.hpp"
#include "Types.hpp"
#include "Timer.hpp"
#include "Lobby.hpp"
#include "PacketParser.hpp"

#include <thread>

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
    Signal<bool> lobbyCreated;
    Signal<uint8_t> joinLobbyAnswer;

    Signal<const std::vector<LobbyData>&> receivedLobbies;
    Signal<const LobbyData&> lobbyUpdate;

    Signal<const std::vector<std::string>&> lobbyPlayersData;

    //TODO we gotta make it better, but Signal class sucks...
    void clearAllSignals()
    {
        connected.clearSlots();
        disconnected.clearSlots();
        failedToConnect.clearSlots();
        lostConnection.clearSlots();
        receivedUDPData.clearSlots();
        receivedTCPData.clearSlots();
        lobbyCreated.clearSlots();
        joinLobbyAnswer.clearSlots();
        receivedLobbies.clearSlots();
        lobbyUpdate.clearSlots();
        lobbyPlayersData.clearSlots();
    }

    Client();

    void update();

    void connect();

    void disconnect();

    void stop();

    void startPing();

    void askForLobbies();

    bool isConnected() const;

    void askToCreateLobby(const LobbyData& lobbyData);

    void joinLobby(uint32_t lobbyId);

    void leaveLobby();

    //TODO for now leave it...
    void authorization(const std::string login);
    bool isAuthorized() const;
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

    std::string m_login;
    bool m_isAuthorized{false};
};

#endif // CLIENT_HPP