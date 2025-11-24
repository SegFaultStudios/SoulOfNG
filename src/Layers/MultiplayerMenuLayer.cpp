#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIInputText.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"

#include "Layers/MainMenuLayer.hpp"
#include "Layers/LobbyLayer.hpp"

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
    auto createLobbyWidgetId = m_scene.addUI<CreateLobbyDialogWidget>("CreateLobbyWidget");
    auto centerizedTextId = m_scene.addUI<UIText>("CenterizedText");
    auto authorizationWidgetId = m_scene.addUI<AuthorizationDialogWidget>("AuthorizationWidget");
 
    auto texture = AssetsManager::instance().loadTexture("./resources/textures/refresh.png");

    auto connectionRotatingImage = m_scene.addUI<RotatingImage>("ConnectionImage", texture);

    m_authorizationWidget = m_scene.getUiWidget<AuthorizationDialogWidget>(authorizationWidgetId);
    m_authorizationWidget->setPosition({sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f)});
    m_authorizationWidget->setSize({m_window.getSize().x / 2.0f - 100.0f, 500});

    m_centerizedText = m_scene.getUiWidget<UIText>(centerizedTextId);

    m_centerizedText->setPosition(sf::Vector2f(m_window.getSize().x / 2.0f - 50.0f, m_window.getSize().y / 2.0f + 50.0f));
    m_centerizedText->setText("Trying to connect");
    m_centerizedText->setCharacterSize(15);

    m_connectionRotatingImage = m_scene.getUiWidget<RotatingImage>(connectionRotatingImage);
    m_connectionRotatingImage->setPosition(sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f));
    m_connectionRotatingImage->setSize({0.1, 0.1});

    m_createLobbyWidget = m_scene.getUiWidget<CreateLobbyDialogWidget>(createLobbyWidgetId);
    m_createLobbyWidget->setSize({m_window.getSize().x / 2.0f - 100.0f, 500});
    m_createLobbyWidget->setPosition({200, 150});

    HANDLE_EVENT(m_authorizationWidget, UIDialogWidget::accepted, this, [this]
    {
        m_client->authorization(m_authorizationWidget->getText());
    });

    HANDLE_EVENT(m_createLobbyWidget, UIDialogWidget::accepted, this, [this]
    {
        const std::string lobbyName = m_createLobbyWidget->getLobbyName();

        LobbyData lobbyData;
        lobbyData.name = lobbyName;
        lobbyData.maxNumberOfPlayers = 4;
        
        m_client->askToCreateLobby(lobbyData);

        m_createLobbyWidget->close();

        m_table->hide();

        m_connectionRotatingImage->show();

        m_centerizedText->show();
        m_centerizedText->setText("Creating lobby...");
    });

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

    HANDLE_EVENT(createLobbyButton, clicked, this, [this]
    {
        if(!m_client->isConnected())
            return;
        
        if(m_createLobbyWidget)
            m_createLobbyWidget->open();
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
        uint32_t lobbyId = static_cast<uint32_t>(ulValue);
        m_client->joinLobby(lobbyId);
    });

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

    if(!m_client->isAuthorized())
    {
        m_authorizationWidget->open();
    }

    HANDLE_EVENT(m_client, Client::connected, this, [this]
    {
        m_connectionRotatingImage->hide();
        m_centerizedText->hide();

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
        m_table->clearRows();
        for(const auto& lobby : lobbyData)
            m_table->addRow({std::to_string(lobby.lobbyId), lobby.name, std::to_string(lobby.numberOfPlayers) + "/" + std::to_string(lobby.maxNumberOfPlayers), "EU"});
    });

    HANDLE_EVENT(m_client, Client::lobbyCreated, this, [this](bool result)
    {
        if(result)
        {
            m_nextLayer = NextMultiplayerMenuLayer::LOBBY;
            std::cout << "Joined to lobby\n";
            this->over();
            // m_connectionRotatingImage->hide();
            // m_centerizedText->hide();
            // m_table->show();

            // m_client->askForLobbies();
        }
    });

    HANDLE_EVENT(m_client, Client::lobbyUpdate, this, [this](const LobbyData& lobbyData)
    {
        //Fucked up...
        for(int rowIndex = 0; rowIndex < m_table->getRows().size(); ++rowIndex)
        {
            const auto& row = m_table->getRows().at(rowIndex);
            const std::string stringId = row.cells.at(0).text.getText();
            unsigned long ulValue = std::stoul(stringId);
            uint32_t lobbyId = static_cast<uint32_t>(ulValue);

            if(lobbyData.lobbyId == lobbyId)
            {
                std::vector<std::string> newRowData;
                // {
                //     std::to_string(lobbyData.lobbyId),
                //     lobbyData.name, 
                //     std::to_string(lobbyData.numberOfPlayers) + "/" + std::to_string(lobbyData.maxNumberOfPlayers), 
                //     "EU"
                // };
                newRowData.push_back(std::to_string(lobbyData.lobbyId));
                newRowData.push_back(lobbyData.name);
                newRowData.push_back(std::to_string(lobbyData.numberOfPlayers) + "/" + std::to_string(lobbyData.maxNumberOfPlayers));
                newRowData.push_back("EU");
                
                m_table->updateRow(rowIndex, newRowData);

                break;
            }
        }
    });

    HANDLE_EVENT(m_client, Client::joinLobbyAnswer, this, [this](uint8_t result)
    {
        if(result != JoinLobbyResponsePacket::JoinLobbyResponseType::SUCCESS)
        {
            std::cerr << "failed to join to lobby\n";
            return;
        }

        std::cout << "Joined to lobby\n";

        m_nextLayer = NextMultiplayerMenuLayer::LOBBY;

        this->over();
    });


    if(!m_client->isConnected())
        m_client->connect();
    else
        m_client->connected.emit(); //Kinda like a hack
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
            m_client->clearAllSignals();
            return std::make_unique<LobbyLayer>(m_window, std::move(m_client));
        }
        default:
            return nullptr;
    }
}