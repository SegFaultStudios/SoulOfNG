#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIInputText.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"

#include "Layers/MainMenuLayer.hpp"
#include "Layers/LobbyLayer.hpp"

#include "Widgets/RotatingImage.hpp"

#include <iostream>

MultiplayerMenuLayer::MultiplayerMenuLayer(sf::RenderWindow& window, std::unique_ptr<SteamClientNetwork> client) : m_window(window)
{
    if(client)
        m_steamClientNetwork = std::move(client);
}

void MultiplayerMenuLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
    m_steamClientNetwork->update();
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
    auto createLobbyWidgetId = m_scene.addUI<CreateLobbyDialogWidget>("CreateLobbyWidget");
    auto centerizedTextId = m_scene.addUI<UIText>("CenterizedText");
    auto authorizationWidgetId = m_scene.addUI<AuthorizationDialogWidget>("AuthorizationWidget");
 
    auto texture = AssetsManager::instance().loadTexture("./resources/textures/refresh.png");

    auto connectionRotatingImage = m_scene.addUI<RotatingImage>("ConnectionImage", texture);

    m_authorizationWidget = m_scene.getUiWidget<AuthorizationDialogWidget>(authorizationWidgetId);
    m_authorizationWidget->setPosition({sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f)});
    m_authorizationWidget->setSize({m_window.getSize().x / 2.0f - 100.0f, 500});
    m_authorizationWidget->hide();

    m_centerizedText = m_scene.getUiWidget<UIText>(centerizedTextId);

    m_centerizedText->setPosition(sf::Vector2f(m_window.getSize().x / 2.0f - 60.0f, m_window.getSize().y / 2.0f + 50.0f));
    m_centerizedText->setText("Trying to connect");
    m_centerizedText->setCharacterSize(15);

    m_connectionRotatingImage = m_scene.getUiWidget<RotatingImage>(connectionRotatingImage);
    m_connectionRotatingImage->setPosition(sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f));
    m_connectionRotatingImage->setSize({0.1, 0.1});

    m_createLobbyWidget = m_scene.getUiWidget<CreateLobbyDialogWidget>(createLobbyWidgetId);
    m_createLobbyWidget->setSize({m_window.getSize().x / 2.0f - 100.0f, 500});
    m_createLobbyWidget->setPosition({200, 150});

    if(SteamBackend::isInitialized())
    {
        m_connectionRotatingImage->hide();
        m_centerizedText->hide();
    }

    auto popupMessageId = m_scene.addUI<UIMessage>("PopUpMessage");

    auto findPrivateLobbyButtonId = m_scene.addUI<UIButton>("FindPrivateLobby");

    auto tableId = m_scene.addUI<UITableWidget>("LobbyTable");

    m_table = m_scene.getUiWidget<UITableWidget>(tableId);

    const std::vector<std::string> headers{"Id", "Lobby", "Players", "Region"};

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

    HANDLE_EVENT(createLobbyButton, UIButton::clicked, this, [this]
    {
        m_steamClientNetwork->createLobby();
    });

    auto joinLobbyButton = m_scene.getUiWidget<UIButton>(joinLobbyButtonId);
    joinLobbyButton->setText("Join lobby");
    joinLobbyButton->setSize({10, 3});
    joinLobbyButton->setPosition({m_window.getSize().x - 450.0f, m_window.getSize().y - 110.0f});
    joinLobbyButton->setTexturedColor(sf::Color(50, 50, 50));

    HANDLE_EVENT(joinLobbyButton, UIButton::clicked, this, [this]
    {
        auto selectedRow = m_table->getSelectedRow();

        if(!selectedRow)
        {
            std::cerr << "table value is not valid\n";
            return;
        }

        //Kinda bad...
        const std::string stringId = selectedRow->cells.at(0).text.getText();
        unsigned long ulValue = std::stoul(stringId);
        uint64 lobbyId = static_cast<uint64>(ulValue);

        m_steamClientNetwork->joinLobby(CSteamID(lobbyId));
    });

    auto findPrivateLobbyButton = m_scene.getUiWidget<UIButton>(findPrivateLobbyButtonId);
    findPrivateLobbyButton->setText("Find lobby");
    findPrivateLobbyButton->setSize({10, 3});
    findPrivateLobbyButton->setPosition({m_window.getSize().x - 675.0f, m_window.getSize().y - 110.0f});
    findPrivateLobbyButton->setTexturedColor(sf::Color(50, 50, 50));

    m_popupMessage = m_scene.getUiWidget<UIMessage>(popupMessageId);
    m_popupMessage->setPosition({m_window.getSize().x - 400.0f, m_window.getSize().y - 200.0f});
    m_popupMessage->getRawText().setCharacterSize(15);

    if(!m_steamClientNetwork)
        m_steamClientNetwork = std::make_unique<SteamClientNetwork>();

    if(!SteamBackend::isInitialized())
        m_centerizedText->setText("Steam is not reachable");

    if(SteamBackend::isInitialized() && m_steamClientNetwork->isConnectedToSteamServers())
        m_steamClientNetwork->requestLobbies();

    HANDLE_SIGNAL(m_steamClientNetwork, lobbyCreated, this, [this](bool isCreated)
    {
        if(isCreated)
        {
            m_nextLayer = NextMultiplayerMenuLayer::LOBBY;
            this->over();
        }
    });

    HANDLE_SIGNAL(m_steamClientNetwork, foundLobbies, this, [this](const std::vector<SteamClientNetwork::SteamLobbyData>& lobbyData)
    {
        m_table->clearRows();
        m_table->show();

        for(const auto& data : lobbyData)
        {
            std::vector<std::string> tableData;
            tableData.push_back(std::to_string(data.id));
            tableData.push_back(data.name);
            tableData.push_back(std::to_string(data.players) + '/' + std::to_string(data.maxPlayers));
            tableData.push_back("UE");
            m_table->addRow(tableData);
        }
    });
}

void MultiplayerMenuLayer::onEnd()
{
}

std::unique_ptr<Layer> MultiplayerMenuLayer::getNextLayer() const
{
    switch(m_nextLayer)
    {
        case NextMultiplayerMenuLayer::BACK_TO_MAIN_MENU:
            return std::make_unique<MainMenuLayer>(m_window);
        case NextMultiplayerMenuLayer::LOBBY:
        {
            m_steamClientNetwork->clearSlots();
            return std::make_unique<LobbyLayer>(m_window, std::move(m_steamClientNetwork));
        }
        default:
            return nullptr;
    }
}