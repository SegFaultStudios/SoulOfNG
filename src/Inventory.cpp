#include "Inventory.hpp"

Inventory::Inventory(const std::string& name) : UIWidget(name)
{    
    sf::Vector2u textureSize(500, 500);
    int cellSize = 5;

    sf::Image image(textureSize, sf::Color::White);

    sf::Color lineColor{sf::Color::Red};
    sf::Color centerLineColor{sf::Color::Black};

    for (unsigned int x = 0; x < textureSize.x; ++x)
    {
        for (unsigned int y = 0; y < textureSize.y; ++y)
        {
            if (x % (cellSize * m_capacity) == 0 || y % (cellSize * m_capacity) == 0)
                image.setPixel({x, y}, centerLineColor);
            
            if (x % (cellSize * m_capacity) == 1 || y % (cellSize * m_capacity) == 1)
                if (x < textureSize.x - 1 && y < textureSize.y - 1)
                    image.setPixel({x, y}, centerLineColor);
        }
    }
    
    m_texture.loadFromImage(image);
    m_texture.setRepeated(true);
    m_sprite = std::make_unique<sf::Sprite>(m_texture);
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!isVisible())
        return;
    
    target.draw(*m_sprite.get(), states);
}   

void Inventory::setPosition(const sf::Vector2f& position) 
{
    UIWidget::setPosition(position);
    m_sprite->setPosition(position);
}

void Inventory::setSize(const sf::Vector2f& size) 
{
    UIWidget::setSize(size);
    m_sprite->setScale(size);
}

sf::FloatRect Inventory::getBoundingBox() const
{
    return m_sprite->getGlobalBounds();
}