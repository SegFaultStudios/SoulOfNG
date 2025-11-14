#include "Layers/SettingsGameLayer.hpp"
#include "Layers/MainMenuLayer.hpp"

#include "UI/UIButton.hpp"
#include "UI/UICheckBox.hpp"
#include "UI/UIText.hpp"

SettingsGameLayer::SettingsGameLayer(sf::RenderWindow& window) : m_window(window)
{

}

void SettingsGameLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
}

void SettingsGameLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);
}

void SettingsGameLayer::handleEvent(sf::Event& event)
{
    m_scene.handleInput(event, m_window);
}

void SettingsGameLayer::onStart()
{
    m_scene.initUiView(m_window);

    //!For now, leave it like that...
    static bool isWindowed = false;

    auto backButtonId = m_scene.addUI<UIButton>("Back");
    auto backButton = m_scene.getUiWidget<UIButton>(backButtonId);
    auto fullscreenTextId = m_scene.addUI<UIText>("FullscreenText");
    auto fullscreenText = m_scene.getUiWidget<UIText>(fullscreenTextId);

    auto fullscreenCheckBoxId = m_scene.addUI<UICheckBox>("FullscreenCheck");
    auto fullscreeenCheckBox = m_scene.getUiWidget<UICheckBox>(fullscreenCheckBoxId);

    fullscreeenCheckBox->setIsChecked(isWindowed);
    fullscreeenCheckBox->setPosition({static_cast<float>(m_window.getSize().x / 2) + 120, 390});
    fullscreeenCheckBox->setSize({10, 10});
    fullscreeenCheckBox->setOnClick([this, fullscreeenCheckBox]
    {
        bool isChecked = fullscreeenCheckBox->isChecked();
        isWindowed = isChecked;

        sf::VideoMode mode = isChecked
            ? sf::VideoMode::getDesktopMode()
            : sf::VideoMode({1280, 720});

        auto state = isChecked
            ? sf::State::Fullscreen
            : sf::State::Windowed;

        m_window.close();
        m_window.create(mode, "SoulfOfNG", sf::Style::Titlebar | sf::Style::Close, state);
        // m_window.setVerticalSyncEnabled(true);
    });

    fullscreenText->setText("Fullscreen");
    fullscreenText->setPosition({static_cast<float>(m_window.getSize().x / 2), 380});
    fullscreenText->setCharacterSize(18);

    const sf::Vector2f buttonSize{7.0, 2};

    backButton->setText("Back");
    backButton->setSize(buttonSize);
    backButton->setPosition({static_cast<float>(m_window.getSize().x / 2), 200});
    backButton->setTextColor(sf::Color::White);
    backButton->setBorderColor(sf::Color::White);
    backButton->setTexturedColor(sf::Color{50, 50, 50});
    backButton->setOnClick([this]
    {
        this->over();
    });
}

void SettingsGameLayer::onEnd()
{

}

std::unique_ptr<Layer> SettingsGameLayer::getNextLayer() const
{
    return std::make_unique<MainMenuLayer>(m_window);
}