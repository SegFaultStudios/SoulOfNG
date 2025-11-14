#ifndef UI_CHECK_BOX_HPP
#define UI_CHECK_BOX_HPP

#include "UI/UIWidget.hpp"

class UICheckBox : public UIWidget
{
public:
    UICheckBox(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void handleCustomEvent(CustomEvent event) override;

    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
    void setIsChecked(bool isChecked);
    bool isChecked() const;
private:
    sf::RectangleShape m_box;
    bool m_isChecked{false};
};

#endif //UI_CHECK_BOX_HPP