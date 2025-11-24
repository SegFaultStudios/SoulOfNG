#ifndef LOBBY_HPP
#define LOBBY_HPP

#include <string>
#include <cstdint>

struct LobbyData
{
    uint32_t lobbyId;
    std::string name;
    uint16_t numberOfPlayers;
    uint16_t maxNumberOfPlayers;
};

#endif //LOBBY_HPP