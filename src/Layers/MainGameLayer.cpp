#include "Layers/MainGameLayer.hpp"

#include "UI/UIButton.hpp"
#include <iostream>
#include "Widgets/Inventory.hpp"
#include "Entities/Player.hpp"

#include "Layers/MainMenuLayer.hpp"

MainGameLayer::MainGameLayer(sf::RenderWindow& window) : m_window(window)
{
}

void MainGameLayer::update(float deltaTime)
{
    sf::Time sfTimeDelta = sf::seconds(deltaTime);

    m_camera->update(deltaTime);
#if USE_EDITOR
    m_editor->update(sfTimeDelta);
#endif 
    m_scene.update(deltaTime);

    if(auto target = m_camera->getTarget())
        m_rayLine[1].position = target->getPosition();
}

void MainGameLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);

#if USE_EDITOR
        m_editor->draw();
#endif

    window.draw(m_rayLine);

    // window.setTitle(std::to_string(1.0f / deltaTime));

    // std::cout << "Frame time: " << deltaTime * 1000.0f << "ms" << std::endl; //How much time took to render a frame
    // std::cout << "FPS: " << 1.0f / deltaTime << std::endl; //FPS counter
}

void MainGameLayer::handleEvent(sf::Event& event)
{
    if(auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if(key->code == sf::Keyboard::Key::Escape)
            m_escapeUI->setVisible(!m_escapeUI->isVisible());
    }

#if USE_EDITOR
    m_editor->processEvents(event);
#endif

    m_camera->handleEvent(event);

    m_scene.handleInput(event, m_window);

    if(auto mouseEvent = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2 mousePosition(mouseEvent->position.x, mouseEvent->position.y);
        auto point = m_window.mapPixelToCoords(mousePosition);
        m_rayLine[0].position = point;
    }
}

void MainGameLayer::onStart()
{
    m_scene.initUiView(m_window);

    m_camera = std::make_unique<Camera>(m_window);

    m_scene.loadFromFile("./resources/maps/default_map.json");

    auto inventoryId = m_scene.addUI<Inventory>("Inventory");

    auto inventory = m_scene.getUiWidget<Inventory>(inventoryId);

    auto escapeUIId = m_scene.addUI<EscapeUI>("EscapeUI");
    m_escapeUI = m_scene.getUiWidget<EscapeUI>(escapeUIId);

    m_escapeUI->setVisible(false);

    m_escapeUI->getMainMenuButton()->setOnClick([this]
    {
        this->over();
    });

    auto playerId = m_scene.findEntityWithName("Player");

    auto player = m_scene.getEntity<Player>(playerId);

    //Create default player??
    if(!player)
    {
        std::cerr << "Failed to find player. Creating default player...\n";
        playerId = m_scene.addEntity<Player>("Player");
        player = m_scene.getEntity<Player>(playerId);
    }
    else
    {
        player->setInventory(inventory);
        m_camera->setTarget(player);
    }

#if USE_EDITOR
    m_editor = std::make_unique<Editor>(m_window, m_scene);
#endif

    m_rayLine.setPrimitiveType(sf::PrimitiveType::Lines);
    m_rayLine.resize(2);
    m_rayLine[0].color = sf::Color::Red;
    m_rayLine[1].color = sf::Color::Green;
}

void MainGameLayer::onEnd()
{

}

std::unique_ptr<Layer> MainGameLayer::getNextLayer() const
{
    return std::make_unique<MainMenuLayer>(m_window);
}