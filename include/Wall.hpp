#ifndef WALL_HPP
#define WALL_HPP

#include "Entity.hpp"

#include <memory>

class Wall : public Entity
{
public:
    Wall(const std::string& name);
    void update(float deltaTime) override;
    void handleInput(const sf::Event& event) override;
};

#endif //WALL_HPP