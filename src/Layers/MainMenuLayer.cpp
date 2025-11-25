#include "Layers/MainMenuLayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/SettingsGameLayer.hpp"
#include "Layers/MultiplayerMenuLayer.hpp"
#include "UI/UIButton.hpp"
#include "AssetsManager.hpp"

MainMenuLayer::MainMenuLayer(sf::RenderWindow& window) : m_window(window)
{

}

void MainMenuLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
}

void MainMenuLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);
}

void MainMenuLayer::handleEvent(sf::Event& event)
{
    m_scene.handleInput(event, m_window);
}

void MainMenuLayer::onStart()
{
    m_scene.initUiView(m_window);

    auto startButtonId = m_scene.addUI<UIButton>("Start");
    auto multiplayerButtonId = m_scene.addUI<UIButton>("Multiplayer");
    auto settingsButtonId = m_scene.addUI<UIButton>("Settings");
    auto exitButtonId = m_scene.addUI<UIButton>("Exit");

    auto settingsButton = m_scene.getUiWidget<UIButton>(settingsButtonId);
    auto startButton = m_scene.getUiWidget<UIButton>(startButtonId);
    auto multiplayerButton = m_scene.getUiWidget<UIButton>(multiplayerButtonId);
    auto exitButton = m_scene.getUiWidget<UIButton>(exitButtonId);

    const sf::Vector2f buttonSize{7.0, 2};

    // const float startY = static_cast<float>(m_window.getSize().y / 2.0f);
    const float startY = 150.0f;
    const float startX = static_cast<float>(m_window.getSize().x / 2.0f);

    startButton->setText("Start");
    startButton->setPosition({startX, startY});
    startButton->setTextColor(sf::Color::White);
    startButton->setBorderColor(sf::Color::White);
    startButton->setTexturedColor(sf::Color{50, 50, 50});
    startButton->setSize(buttonSize);

    HANDLE_EVENT(startButton, UIButton::clicked, this, [this]
    {
        m_nextLayerId = MainMenuNextLayer::SINGLEPLAYER;
        this->over();
    });

    multiplayerButton->setText("Multiplayer");
    multiplayerButton->setSize(buttonSize);
    multiplayerButton->setPosition({startX, startY + 100});
    multiplayerButton->setTextColor(sf::Color::White);
    multiplayerButton->setBorderColor(sf::Color::White);
    multiplayerButton->setTexturedColor(sf::Color{50, 50, 50});
    HANDLE_EVENT(multiplayerButton, UIButton::clicked, this, [this]
    {
        m_nextLayerId = MainMenuNextLayer::MULTIPLAYER;
        this->over();
    });

    settingsButton->setPosition({startX, startY + 200});
    settingsButton->setText("Settings");
    settingsButton->setSize(buttonSize);
    settingsButton->setTextColor(sf::Color::White);
    settingsButton->setBorderColor(sf::Color::White);
    settingsButton->setTexturedColor(sf::Color{50, 50, 50});
    HANDLE_EVENT(settingsButton, UIButton::clicked, this, [this]
    {
        m_nextLayerId = MainMenuNextLayer::SETTINGS;
        this->over();
    });

    exitButton->setPosition({startX, startY + 300});
    exitButton->setText("Exit");
    exitButton->setSize(buttonSize);
    exitButton->setTextColor(sf::Color::White);
    exitButton->setBorderColor(sf::Color::White);
    exitButton->setTexturedColor(sf::Color{50, 50, 50});
    HANDLE_EVENT(exitButton, UIButton::clicked, this, [this]
    {
        m_nextLayerId = MainMenuNextLayer::EXIT;
        this->over();
    });
}

void MainMenuLayer::onEnd()
{

}

std::unique_ptr<Layer> MainMenuLayer::getNextLayer() const
{
    switch(m_nextLayerId)
    {
        case MainMenuNextLayer::MULTIPLAYER:
            return std::make_unique<MultiplayerMenuLayer>(m_window);
        case MainMenuNextLayer::SINGLEPLAYER:
            return std::make_unique<MainGameLayer>(m_window);
        case MainMenuNextLayer::SETTINGS:
            return std::make_unique<SettingsGameLayer>(m_window);
        case MainMenuNextLayer::EXIT:
            return nullptr;
        default:
            return nullptr;
    };
}