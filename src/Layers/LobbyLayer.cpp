#include "Layers/LobbyLayer.hpp"

#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIButton.hpp"
#include "Logger.hpp"
#include <iostream>

LobbyLayer::LobbyLayer(sf::RenderWindow& window, std::unique_ptr<SteamClientNetwork> client) : m_window(window), m_steamClientNetwork(std::move(client))
{

}

void LobbyLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
}

void LobbyLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);
}

void LobbyLayer::handleEvent(sf::Event& event)
{
    m_scene.handleInput(event, m_window);
}

void LobbyLayer::onStart()
{
    m_scene.initUiView(m_window);

    auto lobbyWidgetId = m_scene.addUI<LobbyWidget>("LobbyWidget");
    auto friendsWidgetId = m_scene.addUI<SteamFriendsDialogWidget>("FriendsWidget");
    auto leaveLobbyButtonId = m_scene.addUI<UIButton>("LeaveLobby");
    auto inviteFriendsButtonId = m_scene.addUI<UIButton>("InviteFriends");

    m_friendsWidget = m_scene.getUiWidget<SteamFriendsDialogWidget>(friendsWidgetId);
    m_friendsWidget->setSize({500, 600});
    m_friendsWidget->setPosition({100, 100});

    m_lobbyWidget = m_scene.getUiWidget<LobbyWidget>(lobbyWidgetId);
    m_lobbyWidget->setSize({500, 250});
    m_lobbyWidget->setPosition({m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f});
    m_lobbyWidget->addPlayer(m_steamClientNetwork->getName());

    auto leaveLobbyButton = m_scene.getUiWidget<UIButton>(leaveLobbyButtonId);
    leaveLobbyButton->setText("Leave");
    leaveLobbyButton->setSize({10, 3});
    leaveLobbyButton->setPosition({15.0f, m_window.getSize().y - 110.0f});
    leaveLobbyButton->setTexturedColor(sf::Color{50, 50, 50});

    HANDLE_EVENT(leaveLobbyButton, UIButton::clicked, this, [this]
    {
        m_nextLayer = NextLayer::BACK_TO_MULTIPLAYER_MENU;
        // m_steamClientNetwork->leaveLobby();
        this->over();
    });

    auto inviteFriendsButton = m_scene.getUiWidget<UIButton>(inviteFriendsButtonId);
    inviteFriendsButton->setText("Invite friends");
    inviteFriendsButton->setSize({10, 3});
    inviteFriendsButton->setPosition({m_window.getSize().x / 2.0f - 15.0f, m_window.getSize().y - 110.0f});
    inviteFriendsButton->setTexturedColor(sf::Color{50, 50, 50});

    HANDLE_EVENT(inviteFriendsButton, UIButton::clicked, this, [this]
    {
        auto steamFriends = m_steamClientNetwork->getSteamFriends(SteamClientNetwork::FriendSearch::ONLINE);
        m_friendsWidget->open();
        m_friendsWidget->removeAllFriends();
       
        for(const auto& steamFriend : steamFriends)
        {
            sf::Image avatarImage({steamFriend.avatarWidth, steamFriend.avatarHeight}, steamFriend.avatarData.data());
            sf::Texture avatatTexture;

            if(!avatatTexture.loadFromImage(avatarImage))
            {
                LOG_WARNING("Failed to load avatar texture");
            }

            m_friendsWidget->addFriend(steamFriend.name, avatatTexture, steamFriend.id);
        }
    });

    HANDLE_SIGNAL(m_friendsWidget, inviteFriend, this, [this](CSteamID id)
    {
        m_steamClientNetwork->inviteFriend(id);
    });
}

void LobbyLayer::onEnd()
{

}

std::unique_ptr<Layer> LobbyLayer::getNextLayer() const
{
    switch(m_nextLayer)
    {
        case NextLayer::BACK_TO_MULTIPLAYER_MENU:
        {
            m_steamClientNetwork->clearSlots();
            return std::make_unique<MultiplayerMenuLayer>(m_window, std::move(m_steamClientNetwork));
        }
        default:
            return nullptr;
    }
}