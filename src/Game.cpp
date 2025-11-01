#include "Game.hpp"
#include <filesystem>
#include "UI/UIButton.hpp"

Game::Game(const std::string& gameName)
{
    std::cout << "Loading all textures" << std::endl;

    for(const auto& entry : std::filesystem::recursive_directory_iterator("./resources/textures/"))
        AssetsManager::instance().loadTexture(entry.path().string());

    std::cout << "Loaded successfully " << AssetsManager::instance().getTextures().size() << " textures\n"; 

    m_window.create(sf::VideoMode({800, 600}), gameName, sf::Style::Default);

    sf::Image image({20, 20}, sf::Color::White);
    sf::Texture whiteTexture;

    if(whiteTexture.loadFromImage(image))
        AssetsManager::instance().addTexture(whiteTexture, "default");
    else
        std::cerr << "Failed to load default texture\n";

    m_camera = std::make_unique<Camera>(m_window);

    m_scene.loadFromFile("./resources/maps/default_map.json");

    auto playerId = m_scene.findEntityWithName("Player");

    auto player = m_scene.getEntity(playerId);

    if(!player)
        std::cerr << "Failed to find player\n";
    else
        m_camera->setTarget(player);

    m_scene.addUI<UIButton>("Button");

#if USE_EDITOR
    m_editor = std::make_unique<Editor>(m_window, m_scene);
#endif
}

void Game::run()
{
    sf::Clock clock;
    float deltaTime = 0.0f;

    sf::VertexArray rayLine(sf::PrimitiveType::Lines, 2);
    rayLine[0].color = sf::Color::Red;
    rayLine[1].color = sf::Color::Green;
    
    while (m_window.isOpen())
    {
        sf::Time time = clock.restart();
        deltaTime = time.asSeconds();

        if(auto target = m_camera->getTarget())
            rayLine[1].position = target->getPosition();
        
        while (std::optional event = m_window.pollEvent())
        {
#ifdef USE_EDITOR
            m_editor->processEvents(event.value());
#endif
            m_camera->handleEvent(event.value());

            if (event->is<sf::Event::Closed>())
                m_window.close();

            if(auto mouseEvent = event.value().getIf<sf::Event::MouseMoved>())
            {
                sf::Vector2i mousePosition(mouseEvent->position.x, mouseEvent->position.y);
                auto point = m_window.mapPixelToCoords(mousePosition);
                rayLine[0].position = point;
            }

            m_scene.handleInput(event.value(), m_window);
        }

        m_camera->update(deltaTime);
#ifdef USE_EDITOR
        m_editor->update(time);
#endif 
        m_scene.update(deltaTime);

        m_window.clear();

        m_window.draw(rayLine);

        m_scene.draw(m_window);


#ifdef USE_EDITOR
        m_editor->draw();
#endif
        m_window.display();

        // m_window.setTitle(std::to_string(1.0f / deltaTime));

        // std::cout << "Frame time: " << deltaTime * 1000.0f << "ms" << std::endl; //How much time took to render a frame
        // std::cout << "FPS: " << 1.0f / deltaTime << std::endl; //FPS counter
    }
}
