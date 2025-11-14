#include "Widgets/RotatingImage.hpp"

RotatingImage::RotatingImage(const std::string& name, sf::Texture* texture) : UIWidget(name)
{
    m_sprite = std::make_unique<sf::Sprite>(*texture);

    m_sprite->setOrigin({texture->getSize().x * 0.5f, texture->getSize().y * 0.5f});
}

void RotatingImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // states.transform.translate(getPosition());
    target.draw(*m_sprite.get(), states);
}

void RotatingImage::update(float deltaTime)
{
    UIWidget::update(deltaTime);

    if(m_shoudSpin)
    {
        sf::Angle angle  = sf::degrees(m_rotationSpeed * deltaTime);
        m_sprite->rotate(angle);
    }
}

void RotatingImage::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_sprite->setPosition(position);
}

void RotatingImage::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
    m_sprite->setScale(size);
}

void RotatingImage::stopSpinning()
{
    m_shoudSpin = false;
}

void RotatingImage::startSpinning()
{
    m_shoudSpin = true;
}
