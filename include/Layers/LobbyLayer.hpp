#ifndef LOBBY_LAYER_HPP
#define LOBBY_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"
#include "Network/Client.hpp"

class LobbyLayer : public Layer
{
public:
    LobbyLayer(sf::RenderWindow& window, Client::UniquePtr client);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    Client::UniquePtr m_client{nullptr};
    Scene m_scene;
    sf::RenderWindow& m_window;
};

#endif //LOBBY_LAYER_HPP