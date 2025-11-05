#include "Layers/MainMenuLayer.hpp"

#include "UI/UIButton.hpp"

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
    auto startButtonId = m_scene.addUI<UIButton>("Start");
    auto settingsButtonId = m_scene.addUI<UIButton>("Settings");

    auto settingsButton = m_scene.getUiWidget<UIButton>(settingsButtonId);
    auto startButton = m_scene.getUiWidget<UIButton>(startButtonId);

    startButton->setText("Start");
    startButton->setSize({2.6, 2});
    startButton->setPosition({m_window.getSize().x / 2, 200});
    startButton->setTextColor(sf::Color::White);
    startButton->setBorderColor(sf::Color::White);
    startButton->setTexturedColor(sf::Color{50, 50, 50});
    startButton->setOnClick([this]{this->over();});

    // startButton->setTexture(*AssetsManager::instance().loadTexture("./resources/textures/b1_16x16.png"));

    settingsButton->setPosition({m_window.getSize().x / 2, 300});
    settingsButton->setText("Settings");
    settingsButton->setSize({2.6, 2});
    settingsButton->setTextColor(sf::Color::White);
    settingsButton->setBorderColor(sf::Color::White);
    settingsButton->setTexturedColor(sf::Color{50, 50, 50});
    settingsButton->setTextCharacterSize(10);
}

void MainMenuLayer::onEnd()
{

}