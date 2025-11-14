#ifndef UI_INPUT_TEXT_HPP
#define UI_INPUT_TEXT_HPP

#include "UI/UIWidget.hpp"
#include "UI/UIText.hpp"

class UIInputText : public UIWidget
{
public:
    explicit UIInputText(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(float deltaTime) override;
    sf::FloatRect getBoundingBox() const override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void handleCustomEvent(CustomEvent event) override;
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
    UIText& getText();
private:
    UIText m_text;
    sf::RectangleShape m_background;
    bool m_cursorVisible{true};
    float m_cursorTimer{0.f};
    sf::RectangleShape m_cursor;
    bool m_isFocused{false};
};

#endif //UI_INPUT_TEXT_HPP