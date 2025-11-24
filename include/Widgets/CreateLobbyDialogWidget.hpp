#ifndef CREATE_LOBBY_DIALOG_WIDGET_HPP
#define CREATE_LOBBY_DIALOG_WIDGET_HPP

#include "UI/UIDialogWidget.hpp"
#include "UI/UICheckBox.hpp"
#include "UI/UIInputText.hpp"
#include "UI/UIButton.hpp"

class CreateLobbyDialogWidget : public UIDialogWidget
{
public:
    explicit CreateLobbyDialogWidget(const std::string &name, UIWidget *parent = nullptr);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    void setPosition(const sf::Vector2f &position) override;

    void update(float deltaTime) override;

    std::string getLobbyName();
private:
    UIInputText m_lobbyNameInputText;
    UIButton::UniquePtr m_createLobbyButton{nullptr};
    UIButton::UniquePtr m_backButton{nullptr};
};

#endif // CREATE_LOBBY_DIALOG_WIDGET_HPP