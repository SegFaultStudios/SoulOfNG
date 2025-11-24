#ifndef UI_MESSAGE_HPP
#define UI_MESSAGE_HPP

#include "UI/UIWidget.hpp"
#include "UI/UIText.hpp"

class UIMessage : public UIWidget
{
public:
    explicit UIMessage(const std::string& name, UIWidget* parent = nullptr);

    void update(float deltaTime) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setText(const std::string& text);
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;

    void showFor(float seconds);

    UIText& getRawText();
private:
    void resizeToFitText();

    sf::RectangleShape m_border;
    UIText m_text;
    sf::RectangleShape m_background;
    float m_timer{0.0f};
    float m_duration{0.0f};
    float m_padding{12.0f};
};

#endif //UI_MESSAGE_HPP