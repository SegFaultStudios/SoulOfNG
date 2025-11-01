#ifndef RAY_HPP
#define RAY_HPP

#include <SFML/Graphics.hpp>

struct Ray
{
    sf::Vector2f origin{0.0f, 0.0f};
    sf::Vector2f direction;
    float maxDistance;
};

#endif //RAY_HPP