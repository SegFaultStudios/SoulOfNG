#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity
{
public:
    Player();
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& target) override;
};

#endif //PLAYER_HPP