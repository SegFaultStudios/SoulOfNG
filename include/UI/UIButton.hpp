#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "UI/UIWidget.hpp"

class UIButton : public UIWidget
{
public:
    UIButton(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void handleCustomEvent(CustomEvent event) override;

    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
private:
    sf::RectangleShape m_background;
    sf::Color m_idleColor{sf::Color::White};
    sf::Color m_hoverColor{sf::Color::Red};
    sf::Color m_clickedColor{sf::Color::Green};
};

#endif //UI_BUTTON_HPP