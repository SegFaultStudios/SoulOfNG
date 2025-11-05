#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "UI/UIWidget.hpp"

#include <memory>

class Inventory : public UIWidget
{
public:
    Inventory(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
private:
    int m_capacity = 20;

    std::unique_ptr<sf::Sprite> m_sprite;
    sf::Texture m_texture;
};

#endif //INVENTORY_HPP