#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entities/Entity.hpp"
#include "Scene.hpp"
#include "Widgets/Inventory.hpp"

class Player : public Entity
{
public:
    Player(const std::string& name = "Player");
    void update(float deltaTime) override;
    void handleInput(const sf::Event& event) override;
    void setInventory(Inventory* inventory);
private:
    static constexpr float WALKING_SPEED = 100.0f;
    static constexpr float SPRINTING_SPEED = 200.0f;

    float m_speed{0.0f};

    bool m_isSprinting{false};

    Inventory* m_inventory{nullptr};
};

#endif //PLAYER_HPP