#include "Layers/MainGameLayer.hpp"

#include "UI/UIButton.hpp"
#include <iostream>
#include "Widgets/Inventory.hpp"
#include "Entities/Player.hpp"

#include "Layers/MainMenuLayer.hpp"

#include "Animations/FrameAnimation.hpp"

// static FrameAnimation s_frameAnimation("./resources/animations/Sprite-0003.png");

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

    // s_frameAnimation.update(deltaTime);
}

void MainGameLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);

#if USE_EDITOR
        m_editor->draw();
#endif

    window.draw(m_rayLine);

    // s_frameAnimation.draw(window);

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

    HANDLE_EVENT(m_escapeUI->getMainMenuButton(), UIButton::clicked, this, [this]
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
    m_scene.initQuadTree();
    auto territories = m_BSPGenerator.getTerritories();
    for (auto ter : territories) {
        Room room(ter.position, ter.size);
        auto& roomWalls =room.getWalls();
        for (auto& wall : roomWalls) {
            m_scene.addEntity(std::move(wall));
        }
    }

    // s_frameAnimation.addFrame("./resources/animations/Sprite-0003.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0004.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0005.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0006.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0007.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0008.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0009.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0010.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0011.png");
    // s_frameAnimation.addFrame("./resources/animations/Sprite-0012.png");

    // s_frameAnimation.pause();
}

void MainGameLayer::onEnd()
{

}

std::unique_ptr<Layer> MainGameLayer::getNextLayer() const
{
    return std::make_unique<MainMenuLayer>(m_window);
}