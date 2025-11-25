#include "Game.hpp"
#include "Layers/MainMenuLayer.hpp"

#include <filesystem>
#include "steam/steam_api.h"
//#include "windows.h"

Game::Game(const std::string& gameName)
{
    //SetEnvironmentVariableA("SteamAppId", "480"); //dont commit suka
    //SetEnvironmentVariableA("SteamGameId", "480"); //dont commit suka
    if (!SteamAPI_Init())
        throw std::runtime_error("SteamAPI_Init failed. Make sure Steam is running and your appID is valid");

    auto start = std::chrono::high_resolution_clock::now();

    if(std::filesystem::exists("./resources/textures/"))
    {
        std::cout << "Loading all textures" << std::endl;
        for(const auto& entry : std::filesystem::recursive_directory_iterator("./resources/textures/"))
            AssetsManager::instance().loadTexture(entry.path().string());
        std::cout << "Loaded successfully " << AssetsManager::instance().getTextures().size() << " textures\n"; 
    }
    else
        std::cerr << "ERROR: NO RESOURCES FOLDER" << std::endl;

    m_window.create(sf::VideoMode({1280, 720}), gameName, sf::Style::Titlebar | sf::Style::Close);

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

    m_layer = std::make_unique<MainMenuLayer>(m_window);
    m_layer->onStart();
}

void Game::run()
{

    sf::Clock clock;
    float deltaTime = 0.0f;


    while (m_window.isOpen())
    {
        if(m_layer->isOver())
        {
            auto nextLayer = m_layer->getNextLayer();

            if(nextLayer)
            {
                m_layer->onEnd();
                m_layer.reset();

                m_layer = std::move(nextLayer);
                m_layer->onStart();
            }
            else
            {
                std::cerr << "No more layers. Terminating game..." << std::endl;
                return;
            }
        }

        deltaTime = clock.restart().asSeconds();

        while (auto event = m_window.pollEvent())
        {
            m_layer->handleEvent(event.value());

            if (event->is<sf::Event::Closed>())
                m_window.close();
        }

        m_layer->update(deltaTime);

        m_window.clear();
        m_layer->draw(m_window);
        m_window.display();
    }
}