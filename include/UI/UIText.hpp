#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include "UI/UIWidget.hpp"

#include <memory>

class UIText : public UIWidget
{
public:
    //!LEAVE LIKE THIS FOR NOW
    UIText() : UIText("")
    {
        
    }

    UIText(const std::string& name);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void handleCustomEvent(CustomEvent event) override;
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
    void setText(const std::string& text);
    void setCharacterSize(unsigned int size);
    unsigned int getCharacterSize() const;
    std::string getText() const;

    sf::Text* getRawText() const;
private:
    std::unique_ptr<sf::Text> m_text{nullptr};
    sf::Font m_font;
};

#endif //UI_TEXT_HPP