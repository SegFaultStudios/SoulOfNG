#ifndef LOBBY_HPP
#define LOBBY_HPP

#include <cstdint>
#include <string>
#include <iostream>

#include "Server/ClientConnection.hpp"

class Lobby
{
public:
    uint16_t numberOfPlayers{0};
    uint16_t maxNumberOfPlayers{4};
    uint32_t lobbyId;
    std::string lobbyName;

    const std::vector<ClientConnection*>& getPlayers() const
    {
        return m_players;
    }

    bool removeClient(ClientConnection* clientConnection)
    {
        auto it = std::find_if(m_players.begin(), m_players.end(), [clientConnection](ClientConnection* client)
        {
            return clientConnection == client;
        });

        if(it == m_players.end())
        {
            std::cout << "All players: \n";
            for(const auto& player : m_players)
                std::cout << player << '\n';
            
            std::cout << "Current player: " << clientConnection << '\n';
            return false;
        }

        m_players.erase(it);

        numberOfPlayers = m_players.size();

        return true;
    }

    bool addClient(ClientConnection* clientConnection)
    {
        if(isFull())
            return false;
        
        m_players.push_back(clientConnection);
        numberOfPlayers = m_players.size();

        return true;
    }

    bool isFull() const 
    {
        return m_players.size() >= maxNumberOfPlayers;
    }

    bool isEmpty() const
    {
        return m_players.empty();
    }

private:
    std::vector<ClientConnection*> m_players;
};

#endif //LOBBY_HPP