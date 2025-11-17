#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "UI/UIWidget.hpp"

#include <memory>

//TODO add margin for text

class UIButton : public UIWidget
{
public:
    using UniquePtr = std::unique_ptr<UIButton>;
    
    explicit UIButton(const std::string& name, UIWidget* parent = nullptr);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void handleCustomEvent(CustomEvent event) override;

    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;

    void setTexture(const sf::Texture& texture);
    const sf::Texture& getTexture() const;

    void setText(const std::string& text);
    void setTextColor(const sf::Color& color);
    void setBorderColor(const sf::Color& color);
    void setTexturedColor(const sf::Color& color);
    void setTextCharacterSize(int size);

private:
    void updateTextPosition();

    sf::RectangleShape m_border;
    std::unique_ptr<sf::Sprite> m_sprite{nullptr};
    sf::Font m_font;
    std::unique_ptr<sf::Text> m_text{nullptr};
    sf::Color m_idleColor{sf::Color::White};
    sf::Color m_hoverColor{sf::Color::Red};
    sf::Color m_clickedColor{sf::Color::Green};
};

#endif //UI_BUTTON_HPP