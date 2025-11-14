#ifndef MULTIPLAYER_MENU_CONNECTION_DIALOG_WIDGET_HPP
#define MULTIPLAYER_MENU_CONNECTION_DIALOG_WIDGET_HPP

#include "UI/UIDialogWidget.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "Widgets/RotatingImage.hpp"

#include <SFML/Network.hpp>

#include <future>

class MultiplayerMenuConnectionDialogWidget : public UIDialogWidget
{
public:
    explicit MultiplayerMenuConnectionDialogWidget(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void setPosition(const sf::Vector2f& position) override;
    void update(float deltaTime) override;
private:
    bool tryToConnect();

    std::unique_ptr<UIButton> m_connectButton{nullptr};
    std::unique_ptr<UIButton> m_cancelButton{nullptr};
    std::unique_ptr<RotatingImage> m_rotatingImage{nullptr};
    UIText m_text;

    sf::TcpSocket m_socket;

    std::future<bool> m_connectingFuture;
    bool m_doNotTryToConnect{false};

    bool m_isTryingToConnect{false};
};

#endif //MULTIPLAYER_MENU_CONNECTION_DIALOG_WIDGET_HPP