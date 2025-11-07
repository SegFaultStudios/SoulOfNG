#include "Game.hpp"
#include <filesystem>
#include "UI/UIButton.hpp"

#include "Layers/MainGameLayer.hpp"
#include "Layers/MainMenuLayer.hpp"

Game::Game(const std::string& gameName)
{
    auto start = std::chrono::high_resolution_clock::now();

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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Game loading took: " << duration.count() << " microseconds\n";
    std::cout << "Game loading took: " << duration.count() / 1000.0 << " milliseconds\n";
    std::cout << "Game loading took: " << duration.count() / 1000000.0 << " seconds\n";

    m_layers.emplace_back(std::make_unique<MainMenuLayer>(m_window));
    m_layers.emplace_back(std::make_unique<MainGameLayer>(m_window));

    //Maybe this is not a good decision
    m_currentLayer = m_layers.begin();

    m_currentLayer->get()->onStart();
}

void Game::run()
{

    sf::Clock clock;
    float deltaTime = 0.0f;


    while (m_window.isOpen())
    {
        if(m_currentLayer->get()->isOver())
        {
            if(m_currentLayer == m_layers.end() - 1)
            {
                std::cerr << "No more layers. Terminating game..." << std::endl;
                return;
            }

            m_currentLayer->get()->onEnd();

            ++m_currentLayer;

            m_currentLayer->get()->onStart();
        }

        deltaTime = clock.restart().asSeconds();

        while (auto event = m_window.pollEvent())
        {
            m_currentLayer->get()->handleEvent(event.value());

            if (event->is<sf::Event::Closed>())
                m_window.close();
        }

        m_currentLayer->get()->update(deltaTime);

        m_window.clear();
        m_currentLayer->get()->draw(m_window);
        m_window.display();
    }
}