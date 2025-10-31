#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "UI/UIWidget.hpp"

class UIButton : public UIWidget
{
public:
    UIButton(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
private:
    sf::RectangleShape m_background;
};

#endif //UI_BUTTON_HPP