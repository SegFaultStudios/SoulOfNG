#ifndef AUTHORIZATION_WIDGET_HPP
#define AUTHORIZATION_WIDGET_HPP

#include "UI/UIDialogWidget.hpp"
#include "UI/UIInputText.hpp"
#include "UI/UIButton.hpp"

class AuthorizationDialogWidget : public UIDialogWidget
{
public:
    explicit AuthorizationDialogWidget(const std::string& name, UIWidget* parent = nullptr);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    void setPosition(const sf::Vector2f &position) override;

    void update(float deltaTime) override;

    std::string getText();

private:
    UIInputText m_loginInputText;
    UIButton::UniquePtr m_loginButton{nullptr};
};

#endif //AUTHORIZATION_WIDGET_HPP