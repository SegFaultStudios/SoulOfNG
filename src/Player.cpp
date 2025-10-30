#include "Player.hpp"
#include "SFML/Window/Keyboard.hpp"
Player::Player(const std::string& name) : Entity(name)
{


    m_speed = 200;

}

void Player::handleInput(sf::Event& event)
{
    
}

void Player::setSpeedDefault() {
    m_speed = 200;
}

void Player::update(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
        m_speed = 100;
    } else {
        setSpeedDefault();
    }
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

