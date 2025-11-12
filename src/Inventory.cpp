#include "Inventory.hpp"

#include "AssetsManager.hpp"

#include <iostream>
#include <cmath>

Inventory::Inventory(const std::string& name) : UIWidget(name)
{    
    if (!m_font.openFromFile("./resources/fonts/JetBrainsMono-Bold.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }


    sf::Vector2u textureSize(500, 500);
    int totalCells = m_capacity;
    
    auto calculateGridDimensions = [](int totalCells)-> std::pair<int, int>
    {
        for (int cols = std::sqrt(totalCells); cols >= 1; --cols)
        {
            if (totalCells % cols == 0)
            {
                int rows = totalCells / cols;
                return {cols, rows};
            }
        }

        return {totalCells, 1};
    };
    
    auto [cellsX, cellsY] = calculateGridDimensions(totalCells);

    int cellSizeX = textureSize.x / cellsX;
    int cellSizeY = textureSize.y / cellsY;

    sf::Image image(textureSize, sf::Color(255, 255, 255, 128));
    sf::Color lineColor{sf::Color(0, 0, 0, 200)};

    for (unsigned int x = 0; x < textureSize.x; ++x)
    {
        for (unsigned int y = 0; y < textureSize.y; ++y)
        {
            if (x % cellSizeX == 0) 
                image.setPixel({x, y}, lineColor);
            
            if (y % cellSizeY == 0)
                image.setPixel({x, y}, lineColor);
            
            if (x % cellSizeX == 1 || y % cellSizeY == 1)
                if (x < textureSize.x - 1 && y < textureSize.y - 1)
                    image.setPixel({x, y}, lineColor);
        }
    }

    m_cells.clear();
    m_cells.reserve(cellsX * cellsY);
    
    for (int row = 0; row < cellsY; ++row)
    {
        for (int col = 0; col < cellsX; ++col)
        {
            Cell cell;
            cell.index = row * cellsX + col;
            cell.position = sf::Vector2f(col * cellSizeX, row * cellSizeY);
            cell.size = sf::Vector2f(cellSizeX, cellSizeY);
            cell.bounds = sf::FloatRect(cell.position, cell.size);
            cell.isOccupied = false;
            
            m_cells.push_back(cell);
        }
    }

    if(!m_texture.loadFromImage(image))
        std::cerr << "Failed to create inventory texture" << std::endl;

    m_texture.setRepeated(true);
    m_sprite = std::make_unique<sf::Sprite>(m_texture);
    m_sprite->setColor(sf::Color(255, 255, 255, 200));

    auto& firstCell = m_cells.front();

    firstCell.item = std::make_shared<InventoryItem>();
    firstCell.item->texture = AssetsManager::instance().loadTexture("./resources/textures/b1.png");
    firstCell.item->quantity = 2;
    firstCell.isOccupied = true;
}

void Inventory::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if(!isVisible())
        return;

    if(auto mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(mouseClick->button == sf::Mouse::Button::Left)
        {   
            auto mousePosition = mouseClick->position;

            for(const auto& cell : m_cells)
            {
                if(cell.bounds.contains(sf::Vector2f{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)}))
                {
                    break;
                }
            }
        }
    }
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!isVisible())
        return;
    
    target.draw(*m_sprite.get(), states);

    for(const auto& cell : m_cells)
    {
        if(cell.isOccupied && cell.item)
        {
            sf::Sprite itemSprite(*cell.item->texture);

            sf::FloatRect itemBounds = itemSprite.getLocalBounds();
            sf::Vector2f itemSize(itemBounds.size.x, itemBounds.size.y);

            float scaleX = (cell.size.x * 0.8f) / itemSize.x;
            float scaleY = (cell.size.y * 0.8f) / itemSize.y;
            float scale = std::min(scaleX, scaleY) * m_itemScale;
            
            itemSprite.setScale({scale, scale});
            
            itemSize.x *= scale;
            itemSize.y *= scale;
            
            sf::Vector2f itemPos = cell.position + (cell.size - itemSize) / 2.0f;
            itemSprite.setPosition(itemPos);
                
            target.draw(itemSprite, states);
            
            if (cell.item->quantity > 1)
            {
                sf::Text quantityText(m_font, std::to_string(cell.item->quantity), 12);
                quantityText.setFillColor(sf::Color::White);
                quantityText.setStyle(sf::Text::Bold);
                
                sf::Vector2f textPos = cell.position + cell.size - sf::Vector2f(15, 5);
                quantityText.setPosition(textPos);
                
                target.draw(quantityText, states);
            }
        }
    }
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