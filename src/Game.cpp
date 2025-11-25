#include "Game.hpp"
#include "Layers/MainMenuLayer.hpp"
#include "Network/SteamBackend.hpp"
#include <filesystem>
#include "Logger.hpp"
#include "CrashHandler.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif 

Game::Game(const std::string& gameName)
{
#ifdef _WIN32
    SetEnvironmentVariableA("SteamAppId", "480"); //dont commit suka
    SetEnvironmentVariableA("SteamGameId", "480"); //dont commit suka
#endif

    auto start = std::chrono::high_resolution_clock::now();

    CrashHandler::setupCrashHandler();

    Logger::createDefaultLogger();

    if(!SteamBackend::init())
        std::cerr << "Failed to init STEAM client. Steam features will be disabled\n";

    if(std::filesystem::exists("./resources/textures/"))
    {
        LOG_DEBUG("Loading all textures");
        for(const auto& entry : std::filesystem::recursive_directory_iterator("./resources/textures/"))
            AssetsManager::instance().loadTexture(entry.path().string());
        LOG_DEBUG("Loaded successfully " + std::to_string(AssetsManager::instance().getTextures().size()) + " textures");
    }
    else
        LOG_WARNING("No resources folder");

    m_window.create(sf::VideoMode({1280, 720}), gameName, sf::Style::Titlebar | sf::Style::Close);

    sf::Image image({20, 20}, sf::Color::White);
    sf::Texture whiteTexture;

    if(whiteTexture.loadFromImage(image))
        AssetsManager::instance().addTexture(whiteTexture, "default");
    else
        std::cerr << "Failed to load default texture\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    LOG_DEBUG("Game loading took: " + std::to_string(duration.count()) + " microseconds");
    LOG_DEBUG("Game loading took: " + std::to_string(duration.count() / 1000.0) + " milliseconds");
    LOG_DEBUG("Game loading took: " + std::to_string(duration.count() / 1000000.0) + " seconds");

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
                LOG_WARNING("No more layers. Terminating game...");
                break;
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

void Game::cleanup()
{
    SteamBackend::destroy();
}