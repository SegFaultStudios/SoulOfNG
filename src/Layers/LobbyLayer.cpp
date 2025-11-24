#include "Layers/LobbyLayer.hpp"

#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIButton.hpp"

#include <iostream>

LobbyLayer::LobbyLayer(sf::RenderWindow& window, Client::UniquePtr client) : m_window(window), m_client(std::move(client))
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
    auto leaveLobbyButtonId = m_scene.addUI<UIButton>("LeaveLobby");

    m_lobbyWidget = m_scene.getUiWidget<LobbyWidget>(lobbyWidgetId);
    m_lobbyWidget->setSize({500, 250});
    m_lobbyWidget->setPosition({m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f});
    // m_lobbyWidget->addPlayer("Dlyvern");
    m_lobbyWidget->addChatMessage("Dlyvern joined");

    auto leaveLobbyButton = m_scene.getUiWidget<UIButton>(leaveLobbyButtonId);
    leaveLobbyButton->setText("Leave");
    leaveLobbyButton->setSize({10, 3});
    leaveLobbyButton->setPosition({15.0f, m_window.getSize().y - 110.0f});
    leaveLobbyButton->setTexturedColor(sf::Color{50, 50, 50});

    HANDLE_EVENT(leaveLobbyButton, UIButton::clicked, this, [this]
    {
        m_nextLayer = NextLayer::BACK_TO_MULTIPLAYER_MENU;
        m_client->leaveLobby();
        this->over();
    });

    HANDLE_EVENT(m_client, Client::lobbyPlayersData, this, [this](const std::vector<std::string>& playerNames)
    {
        std::cout << "New player\n";

        m_lobbyWidget->clearPlayers();

        for(const auto& playerName : playerNames)
            m_lobbyWidget->addPlayer(playerName);
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
            m_client->clearAllSignals();
            return std::make_unique<MultiplayerMenuLayer>(m_window, std::move(m_client));
        }
        default:
            return nullptr;
    }
}