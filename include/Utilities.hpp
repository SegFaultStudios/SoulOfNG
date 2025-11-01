#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

namespace utilities
{
    inline sf::Vector2f normalize(const sf::Vector2f& direction)
    {
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (magnitude != 0.f)
            return sf::Vector2f(direction.x / magnitude, direction.y / magnitude);
        else
            return sf::Vector2f(0.f, 0.f); 
    }

    inline float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        sf::Vector2f delta = b - a;

        float squaredDistance = pow(delta.x, 2.0f) * pow(delta.y, 2.0f);

        float distance = std::sqrt(squaredDistance);

        return distance;
    }
} //namespace utilities

#endif //UTILITIES_HPP