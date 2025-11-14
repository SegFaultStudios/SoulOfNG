#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "UI/UIWidget.hpp"

#include <memory>

class Inventory : public UIWidget
{
public:
    struct InventoryItem
    {
        sf::Texture* texture{nullptr};
        int quantity = 1;
    };

    struct Cell
    {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::FloatRect bounds;
        unsigned int index;
        bool isOccupied{false};
        std::shared_ptr<InventoryItem> item{nullptr};
    };

    Inventory(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
private:
    sf::Font m_font;
    int m_capacity = 20;

    float m_itemScale{0.7f};

    std::vector<Cell> m_cells;
    std::unique_ptr<sf::Sprite> m_sprite;
    sf::Texture m_texture;
};

#endif //INVENTORY_HPP