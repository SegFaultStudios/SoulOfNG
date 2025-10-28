#include "Player.hpp"

Player::Player(const std::string& name) : Entity(name)
{
    m_playerMesh.setSize({50, 50});

    m_playerMesh.setPosition({400, 300});
    m_playerMesh.setFillColor(sf::Color::Green);

    setScale({50, 50});
    setPosition({400, 300});
}

void Player::handleInput(sf::Event& event)
{

}

void Player::update(float deltaTime)
{

}

void Player::draw(sf::RenderWindow& target)
{
    target.draw(m_playerMesh);
}