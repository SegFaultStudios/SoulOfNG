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
    //m_potentialPosition = this->getGlobalBounds().position;
    m_potentialMove = { 0, 0 };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        //setPosition({ getPosition().x + m_speed * deltaTime, getPosition().y });
        //m_potentialPosition += sf::Vector2f(m_speed * deltaTime, 0.0f);
        m_potentialMove += sf::Vector2f(m_speed * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        //setPosition({getPosition().x - m_speed * deltaTime, getPosition().y});
        //m_potentialPosition += sf::Vector2f(-m_speed * deltaTime, 0.0f);
        m_potentialMove += sf::Vector2f(-m_speed * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        //setPosition({getPosition().x, getPosition().y  - m_speed * deltaTime});
        //m_potentialPosition += sf::Vector2f(0.0f, -m_speed * deltaTime);
        m_potentialMove += sf::Vector2f(0.0f, -m_speed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        //setPosition({getPosition().x, getPosition().y + m_speed * deltaTime});
        //m_potentialPosition += sf::Vector2f(0.0f, m_speed * deltaTime);
        m_potentialMove += sf::Vector2f(0.0f, m_speed * deltaTime);
    }
    //if (m_potentialPosition != potentialMove) {
        potentialMoveChanged.emit(m_potentialMove);
    //}
 }

sf::Vector2f Player::getPotentialMove() const {
    return m_potentialMove;
}