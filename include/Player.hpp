#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity
{
public:
    Player(const std::string& name = "Player");
    void update(float deltaTime) override;
    void handleInput(sf::Event& event) override;

    void setSpeedDefault();

private:
    float m_speed;

};

#endif //PLAYER_HPP