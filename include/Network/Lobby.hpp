#ifndef LOBBY_HPP
#define LOBBY_HPP

#include <string>
#include <cstdint>

struct LobbyData
{
    std::string name;
    uint16_t numberOfPlayers;
    uint16_t maxNumberOfPlayers;
};

#endif //LOBBY_HPP