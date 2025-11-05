//
// Created by Dmitrii on 03.11.2025.
//
#include "Room.hpp"
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>
//remove iostream
Room::Room(const sf::Vector2f& position, const sf::Vector2f& size)
    : m_position(position), m_size(size)
{
    debugTextureSize();
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

void Room::debugTextureSize() {
    auto testWall = std::make_unique<Entity>("testWall");
    testWall->setPosition({0, 0});
    testWall->setScale({2.0f, 3.0f});


    auto bounds = testWall->getGlobalBounds();
    std::cout <<"Center: " << bounds.getCenter().x
    << std::endl << bounds.getCenter().y << std::endl;
}

