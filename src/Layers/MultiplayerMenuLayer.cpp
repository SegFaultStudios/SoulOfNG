#include "Layers/MultiplayerMenuLayer.hpp"

#include "UI/UIInputText.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"

#include "Layers/MainMenuLayer.hpp"
#include "Widgets/RotatingImage.hpp"

#include <iostream>

MultiplayerMenuLayer::MultiplayerMenuLayer(sf::RenderWindow& window) : m_window(window)
{

}

void MultiplayerMenuLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);

    if(auto result = m_dialogWidget->getResult(); result != UIDialogWidget::DialogResult::None)
    {
        if(result == UIDialogWidget::DialogResult::Rejected)
        {
            m_nextLayer = NextMultiplayerMenuLayer::BACK_TO_MAIN_MENU;
            this->over();
        }
    }
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
    auto dialogWidgetId = m_scene.addUI<MultiplayerMenuConnectionDialogWidget>("Dialog");
    auto tableId = m_scene.addUI<UITableWidget>("LobbyTable");

    m_table = m_scene.getUiWidget<UITableWidget>(tableId);


    const std::vector<std::string> headers{"Lobby", "Players", "Region"};

    m_table->setColumnCount(headers.size());
    m_table->setHeaderLabels(headers);
    m_table->addRow({"Lobby #1", "2/4", "EU"});
    m_table->addRow({"Lobby #2", "4/4", "US"});
    m_table->addRow({"Lobby #3", "1/4", "Asia"});

    m_table->setPosition({0.0f, 0.0f});
    m_table->setSize({m_window.getSize().x / 1.0f, m_window.getSize().y / 1.0f});

    m_table->setColumnWidth(0, 300.0f);

    m_dialogWidget = m_scene.getUiWidget<MultiplayerMenuConnectionDialogWidget>(dialogWidgetId);
    m_dialogWidget->setTitle("Connect to server");
    m_dialogWidget->setSize({400, 250});
    m_dialogWidget->setPosition({200, 150});
    m_dialogWidget->open();
    
    auto backButton = m_scene.getUiWidget<UIButton>(backButtonId);
    backButton->setText("Back");
    backButton->setSize({5, 3});
    backButton->setPosition({15.0f, m_window.getSize().y - 110.0f});
    backButton->setTextColor(sf::Color::White);
    backButton->setBorderColor(sf::Color::White);
    backButton->setTexturedColor(sf::Color{50, 50, 50});
    backButton->setOnClick([this]
    {
        m_nextLayer = NextMultiplayerMenuLayer::BACK_TO_MAIN_MENU;
        this->over();
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
        default:
            return nullptr;
    }
}