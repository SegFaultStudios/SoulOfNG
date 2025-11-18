#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIInputText.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"

#include "Layers/MainMenuLayer.hpp"
#include "Widgets/RotatingImage.hpp"

#include <iostream>

MultiplayerMenuLayer::MultiplayerMenuLayer(sf::RenderWindow& window, Client::UniquePtr client) : m_window(window)
{
    if(client)
        m_client = std::move(client);
}

void MultiplayerMenuLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
    m_client->update();
}

void MultiplayerMenuLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);
}

void MultiplayerMenuLayer::handleEvent(sf::Event& event)
{
    m_scene.handleInput(event, m_window);
}

void MultiplayerMenuLayer::onStart()
{
    m_scene.initUiView(m_window);

    auto backButtonId = m_scene.addUI<UIButton>("Back");
    auto createLobbyButtonId = m_scene.addUI<UIButton>("CreateLobby");
    auto joinLobbyButtonId = m_scene.addUI<UIButton>("JoinLobby");
 
    auto texture = AssetsManager::instance().loadTexture("./resources/textures/refresh.png");

    auto connectionRotatingImage = m_scene.addUI<RotatingImage>("ConnectionImage", texture);

    m_connectionRotatingImage = m_scene.getUiWidget<RotatingImage>(connectionRotatingImage);
    m_connectionRotatingImage->setPosition(sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f));
    m_connectionRotatingImage->setSize({0.1, 0.1});

    auto popupMessageId = m_scene.addUI<UIMessage>("PopUpMessage");

    auto findPrivateLobbyButtonId = m_scene.addUI<UIButton>("FindPrivateLobby");

    auto tableId = m_scene.addUI<UITableWidget>("LobbyTable");

    m_table = m_scene.getUiWidget<UITableWidget>(tableId);

    const std::vector<std::string> headers{"Lobby", "Players", "Region"};

    m_table->setColumnCount(headers.size());
    m_table->setHeaderLabels(headers);

    m_table->setPosition({0.0f, 0.0f});
    m_table->setSize({m_window.getSize().x / 1.0f, m_window.getSize().y / 1.0f});

    float widthPerColumn = m_window.getSize().x / headers.size();
    
    for(int index = 0; index < headers.size(); ++index)
        m_table->setColumnWidth(index, widthPerColumn);

    m_table->hide();

    auto backButton = m_scene.getUiWidget<UIButton>(backButtonId);
    backButton->setText("Back");
    backButton->setSize({10, 3});
    backButton->setPosition({15.0f, m_window.getSize().y - 110.0f});
    backButton->setTexturedColor(sf::Color{50, 50, 50});

    HANDLE_EVENT(backButton, clicked, this, [this]
    {
        m_nextLayer = NextMultiplayerMenuLayer::BACK_TO_MAIN_MENU;
        this->over();
    });

    auto createLobbyButton = m_scene.getUiWidget<UIButton>(createLobbyButtonId);
    createLobbyButton->setText("Create lobby");
    createLobbyButton->setSize({10, 3});
    createLobbyButton->setPosition({m_window.getSize().x - 225.0f, m_window.getSize().y - 110.0f});
    createLobbyButton->setTexturedColor(sf::Color(50, 50, 50));

    HANDLE_EVENT(createLobbyButton, clicked, this, [this]
    {
    });

    auto joinLobbyButton = m_scene.getUiWidget<UIButton>(joinLobbyButtonId);
    joinLobbyButton->setText("Join lobby");
    joinLobbyButton->setSize({10, 3});
    joinLobbyButton->setPosition({m_window.getSize().x - 450.0f, m_window.getSize().y - 110.0f});
    joinLobbyButton->setTexturedColor(sf::Color(50, 50, 50));

    auto findPrivateLobbyButton = m_scene.getUiWidget<UIButton>(findPrivateLobbyButtonId);
    findPrivateLobbyButton->setText("Find lobby");
    findPrivateLobbyButton->setSize({10, 3});
    findPrivateLobbyButton->setPosition({m_window.getSize().x - 675.0f, m_window.getSize().y - 110.0f});
    findPrivateLobbyButton->setTexturedColor(sf::Color(50, 50, 50));

    m_popupMessage = m_scene.getUiWidget<UIMessage>(popupMessageId);
    m_popupMessage->setPosition({m_window.getSize().x - 400.0f, m_window.getSize().y - 200.0f});
    m_popupMessage->getRawText().setCharacterSize(15);

    if(!m_client)
        m_client = std::make_unique<Client>();

    HANDLE_EVENT(m_client, Client::connected, this, [this]
    {
        m_connectionRotatingImage->hide();

        m_table->show();

        m_client->startPing();

        m_client->askForLobbies();
    });

    HANDLE_EVENT(m_client, Client::failedToConnect, this, [this]
    {
        m_popupMessage->setText("Failed to connect to server");
        m_popupMessage->showFor(3);
        m_client->connect();
    });

    HANDLE_EVENT(m_client, Client::receivedLobbies, this, [this](const std::vector<LobbyData>& lobbyData)
    {        
        for(const auto& lobby : lobbyData)
            m_table->addRow({lobby.name, std::to_string(lobby.numberOfPlayers) + "/" + std::to_string(lobby.maxNumberOfPlayers), "EU"});
    });

    m_client->connect();
}

void MultiplayerMenuLayer::onEnd()
{
    m_client->stop();
}

std::unique_ptr<Layer> MultiplayerMenuLayer::getNextLayer() const
{
    switch(m_nextLayer)
    {
        case NextMultiplayerMenuLayer::BACK_TO_MAIN_MENU:
            return std::make_unique<MainMenuLayer>(m_window);
        default:
            return nullptr;
    }
}