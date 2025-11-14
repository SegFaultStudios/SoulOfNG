#ifndef ESCAPE_UI
#define ESCAPE_UI

#include "UI/UIWidget.hpp"
#include "UI/UIButton.hpp"

class EscapeUI : public UIWidget
{
public:
    explicit EscapeUI(const std::string& name);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingBox() const override;
    void setPosition(const sf::Vector2f& position) override;
    void setSize(const sf::Vector2f& size) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    UIButton* getMainMenuButton() const;
private:
    sf::RectangleShape m_background;
    UIButton::UniquePtr m_backToMainMenuButton{nullptr};
};

#endif //ESCAPE_UI