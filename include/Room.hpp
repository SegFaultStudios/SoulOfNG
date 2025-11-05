#ifndef SOUL_OF_NG_ROOM_H
#define SOUL_OF_NG_ROOM_H

#include <vector>
#include<Entity.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>


class Room {
public:
    enum class ExitSide {
        TOP, RIGHT, BOTTOM, LEFT
    };
    Room(const sf::Vector2f& position, const sf::Vector2f&size);
    void generateExits();

    void debugTextureSize();



private:

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    std::vector<ExitSide> m_exits;


};

#endif //SOUL_OF_NG_ROOM_H