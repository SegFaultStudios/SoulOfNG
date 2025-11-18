#include "Layers/LobbyLayer.hpp"

LobbyLayer::LobbyLayer(sf::RenderWindow& window, Client::UniquePtr client) : m_window(window), m_client(std::move(client))
{
    
}

void LobbyLayer::update(float deltaTime)
{
    m_scene.update(deltaTime);
}

void LobbyLayer::draw(sf::RenderWindow& window)
{
    m_scene.draw(window);
}

void LobbyLayer::handleEvent(sf::Event& event)
{
    m_scene.handleInput(event, m_window);
}

void LobbyLayer::onStart()
{

}

void LobbyLayer::onEnd()
{

}

std::unique_ptr<Layer> LobbyLayer::getNextLayer() const
{
    return nullptr;
}