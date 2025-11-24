#ifndef SOUL_OF_NG_ROOM_H
#define SOUL_OF_NG_ROOM_H

#include <vector>
#include "Entities/Wall.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>


class Room {
public:
    enum class ExitSide {
        TOP, RIGHT, BOTTOM, LEFT
    };

    enum class TypeAnchor {
        SOLID_WALL, CORNER, WALL_SMALL, EXIT
    };

private:
    struct WallAnchor {
        TypeAnchor w_type;
        sf::Vector2f w_position;
        sf::Vector2f w_size;
        WallAnchor( TypeAnchor type, sf::Vector2f position, sf::Vector2f size)
            : w_type(type), w_position(position), w_size(size) {};
    };
    float m_epsilon = 1.0f;

public:

    Room(const sf::Vector2f& position, const sf::Vector2f&size);
    void generateExits();
    void addAnchorsForHorizontal(bool doesHaveExit, float positionY);
    void addAnchorsForVertical(bool doesHaveExit, float positionX);
    void addAnchors();
    void addExitAnchor(float posX, float posY);
    void addCornerAnchor(sf::Vector2f pos);

    void createWalls();
    std::unique_ptr<Wall> createWallFromAnchor(const Room::WallAnchor& anchor);


   // void debugTextureSize();

    std::vector<std::unique_ptr<Wall>>& getWalls();




private:
    float m_exitWidth;
    float m_wallThickness;
    std::vector<WallAnchor> m_anchors;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    std::vector<ExitSide> m_exits;
    std::vector<std::unique_ptr<Wall>> m_walls;
    static float half(float value) { return value / 2.0f; }


};

#endif //SOUL_OF_NG_ROOM_H