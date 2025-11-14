#include "Entities/Player.hpp"
#include "SFML/Window/Keyboard.hpp"

Player::Player(const std::string& name) : Entity(name)
{
    m_speed = WALKING_SPEED;
}

void Player::handleInput(const sf::Event& event)
{
    if(auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if(key->code == sf::Keyboard::Key::Tab)
            m_inventory->setVisible(!m_inventory->isVisible());
        
        if(key->code == sf::Keyboard::Key::LShift && !m_isSprinting)
        {
            m_isSprinting = true;
            m_speed = SPRINTING_SPEED;
        }
    }

    if(auto key = event.getIf<sf::Event::KeyReleased>())
    {
        if(key->code == sf::Keyboard::Key::LShift && m_isSprinting)
        {
            m_speed = WALKING_SPEED;
            m_isSprinting = false;
        }
    }
}

void Player::setInventory(Inventory* inventory)
{
    m_inventory = inventory;
    m_inventory->hide();
}

void Player::update(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        setPosition({getPosition().x  + m_speed * deltaTime, getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        setPosition({getPosition().x - m_speed * deltaTime, getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        setPosition({getPosition().x, getPosition().y  - m_speed * deltaTime});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        setPosition({getPosition().x, getPosition().y + m_speed * deltaTime});
    }
}