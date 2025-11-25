#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entities/Entity.hpp"
#include "Scene.hpp"
#include "QuadTree.hpp"
#include "Widgets/Inventory.hpp"
#include "Signal.hpp"

class Player : public Entity
{
public:
    Player(const std::string& name = "Player");
    void update(float deltaTime) override;
    void handleInput(const sf::Event& event) override;
    void setInventory(Inventory* inventory);
    sf::Vector2f getPotentialMove() const;
    Signal<const sf::Vector2f&> potentialMoveChanged;
private:
    static constexpr float WALKING_SPEED = 100.0f;
    static constexpr float SPRINTING_SPEED = 200.0f;
    sf::Vector2f m_potentialMove;
    QuadTree* m_quadTree;


    float m_speed{0.0f};

    bool m_isSprinting{false};

    Inventory* m_inventory{nullptr};
};

#endif //PLAYER_HPP