
#include "Room.hpp"
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>
//remove iostream
Room::Room(const sf::Vector2f& position, const sf::Vector2f& size)
    : m_position(position), m_size(size)
{
    m_exitWidth = 60.0f; /*(m_size.x < m_size.y ? m_size.x : m_size.y) / 4.0f;*/
    m_wallThickness = 30.0f; /*(m_size.x < m_size.y ? m_size.x : m_size.y) / 8.0f;*/
    generateExits();
    addAnchors();
    createWalls();
    //debugTextureSize();
}

void Room::createWalls() {
    m_walls.clear();
    for (const auto& anchor : m_anchors) {
        if (anchor.w_type != TypeAnchor::EXIT) {
            m_walls.push_back(std::move(createWallFromAnchor(anchor)));
        }
    }
}

std::unique_ptr<Wall> Room::createWallFromAnchor(const Room::WallAnchor& anchor) {
    auto wall = std::make_unique<Wall>("Room_wall");
    auto bounds = wall->getGlobalBounds();
    auto center = bounds.getCenter();
    wall->setOrigin(center);
    wall->setPosition(anchor.w_position);
    wall->setScale({(anchor.w_size.x) / 20, (anchor.w_size.y) / 20});
    if (anchor.w_position.x == 0.0f && anchor.w_position.y == 0) {
        std::cout << "pos of First wall at 0;0  " << anchor.w_position.x << anchor.w_position.y << std::endl;
        std::cout << "getGlob   " << wall->getGlobalBounds().position.x << " " << wall->getGlobalBounds().position.y << std::endl;
    }

    return wall;
}

void Room::generateExits() {
    m_exits.clear();

    std::vector<ExitSide> allSides = {
        ExitSide::TOP,
        ExitSide::RIGHT,
        ExitSide::BOTTOM,
        ExitSide::LEFT
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::ranges::shuffle(allSides.begin(), allSides.end(), gen);
    m_exits.push_back(allSides[0]);
    m_exits.push_back(allSides[1]);


}

void Room::addAnchorsForHorizontal(bool doesHaveExit, float positionY) {

    if (doesHaveExit) {

        float wallSizeX = half(m_size.x - m_exitWidth - m_wallThickness) + m_epsilon;
        float firstWallPosX = m_position.x + half(m_wallThickness + wallSizeX);
        float exitPosX = m_position.x + half(m_size.x);
        float secondWallPosX = m_position.x + half(m_size.x + m_exitWidth + wallSizeX);

        m_anchors.emplace_back(TypeAnchor::WALL_SMALL,
            sf::Vector2f( firstWallPosX, positionY),
            sf::Vector2f(wallSizeX, m_wallThickness));

        addExitAnchor(exitPosX, positionY);

        m_anchors.emplace_back(TypeAnchor::WALL_SMALL,
            sf::Vector2f(secondWallPosX, positionY),
            sf::Vector2f(wallSizeX, m_wallThickness));

    } else {
        float wallSizeX = m_size.x  - m_wallThickness + m_epsilon;
        float wallPosX = m_position.x + half(m_size.x);

        m_anchors.emplace_back(TypeAnchor::SOLID_WALL,
            sf::Vector2f(wallPosX, positionY),
            sf::Vector2f(wallSizeX, m_wallThickness));
    }
}

void Room::addAnchorsForVertical(bool doesHaveExit, float positionX) {
    if (doesHaveExit) {
        float wallSizeY = half(m_size.y - m_exitWidth - m_wallThickness) + m_epsilon;
        float firstWallPosY = m_position.y + half(m_wallThickness + wallSizeY);
        float exitPosY = m_position.y + half(m_size.y);
        float secondWallPosY = m_position.y + half(m_size.y + m_exitWidth + wallSizeY);

        m_anchors.emplace_back(TypeAnchor::WALL_SMALL,
            sf::Vector2f(positionX, firstWallPosY),
            sf::Vector2f(m_wallThickness, wallSizeY));

        addExitAnchor(positionX, exitPosY);

        m_anchors.emplace_back(TypeAnchor::WALL_SMALL,
            sf::Vector2f(positionX, secondWallPosY),
            sf::Vector2f(m_wallThickness, wallSizeY));

    } else {
        float wallSizeY = m_size.y - m_wallThickness + m_epsilon;
        float wallPosY = m_position.y + half(m_size.y);

        m_anchors.emplace_back(TypeAnchor::SOLID_WALL,
            sf::Vector2f(positionX, wallPosY),
            sf::Vector2f(m_wallThickness, wallSizeY));
    }
}



void Room::addAnchors() {

    m_anchors.clear();

    addCornerAnchor({m_position.x, m_position.y});
    addCornerAnchor({m_position.x + m_size.x, m_position.y});
    addCornerAnchor({m_position.x, m_position.y + m_size.y});
    addCornerAnchor({m_position.x + m_size.x, m_position.y + m_size.y});

    auto hasExit = [this](ExitSide side) {
        return
            std::ranges::find(m_exits, side) != m_exits.end();
    };

    addAnchorsForHorizontal(hasExit(ExitSide::TOP), m_position.y);

    addAnchorsForHorizontal(hasExit(ExitSide::BOTTOM), m_position.y + m_size.y);

    addAnchorsForVertical(hasExit(ExitSide::LEFT), m_position.x);

    addAnchorsForVertical(hasExit(ExitSide::RIGHT), m_position.x + m_size.x);

}

void Room::addCornerAnchor(sf::Vector2f pos) {
    m_anchors.emplace_back(TypeAnchor::CORNER,
        sf::Vector2f(pos.x, pos.y),
        sf::Vector2f(m_wallThickness, m_wallThickness));
}

void Room::addExitAnchor(float posX, float posY) {
    m_anchors.emplace_back(TypeAnchor::EXIT,
        sf::Vector2f(posX, posY),
        sf::Vector2f(0, 0));
}

/*void Room::debugTextureSize() {
    auto testWall = std::make_unique<Wall>("testWall");
    testWall->setPosition({0, 0});
    testWall->setScale({2.0f, 3.0f});

    auto bounds = testWall->getGlobalBounds();
    std::cout <<"Center: " << bounds.getCenter().x
    << std::endl << bounds.getCenter().y << std::endl;

    m_walls.push_back(std::move(testWall));

}*/

std::vector<std::unique_ptr<Wall>>& Room::getWalls() {
    return m_walls;

}



