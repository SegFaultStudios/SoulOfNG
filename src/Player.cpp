#include "Player.hpp"

static sf::RectangleShape player(sf::Vector2f(50, 50));

Player::Player()
{
    player.setPosition({400, 300});
    player.setFillColor(sf::Color::Green);

}

void Player::update(float deltaTime)
{

}

void Player::draw(sf::RenderWindow& target)
{
    target.draw(player);
}