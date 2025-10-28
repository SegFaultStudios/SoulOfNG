#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& target) = 0;
    virtual ~Entity() = default;

    sf::Vector2f position;
};

#endif //ENTITY_HPP