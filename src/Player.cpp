#include "Player.hpp"
#include "SFML/Window/Keyboard.hpp"
Player::Player(const std::string& name) : Entity(name)
{
    m_playerMesh.setSize({50, 50});

    m_playerMesh.setPosition({400, 300});
    m_playerMesh.setFillColor(sf::Color::Green);
    m_playerMesh.setOrigin({25, 25});

    setScale({50, 50});
    setPosition({400, 300});

    m_speed = 200;
}

void Player::handleInput(sf::Event& event)
{

}

void Player::speedSetDefault() {
    m_speed = 200;
}

void Player::update(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
        m_speed = 100;
    } else {
        speedSetDefault();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        setPosition({getPosition().x  + m_speed * deltaTime, getPosition().y});
        m_playerMesh.setPosition({getPosition().x, getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        setPosition({getPosition().x - m_speed * deltaTime, getPosition().y});
        m_playerMesh.setPosition({getPosition().x, getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        setPosition({getPosition().x, getPosition().y  - m_speed * deltaTime});
        m_playerMesh.setPosition({getPosition().x, getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        setPosition({getPosition().x, getPosition().y + m_speed * deltaTime});
        m_playerMesh.setPosition({getPosition().x, getPosition().y});
    }
}

void Player::draw(sf::RenderWindow& target)
{
    target.draw(m_playerMesh);
}