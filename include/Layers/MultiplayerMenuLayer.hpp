#ifndef MULTIPLAYER_MENU_LAYER_HPP
#define MULTIPLAYER_MENU_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"

#include "Widgets/RotatingImage.hpp"

#include "UI/UITableWidget.hpp"
#include "UI/UIMessage.hpp"

#include "Network/Client.hpp"

class MultiplayerMenuLayer : public Layer
{
public:
    explicit MultiplayerMenuLayer(sf::RenderWindow& window, Client::UniquePtr client = nullptr);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    Client::UniquePtr m_client{nullptr};

    enum class NextMultiplayerMenuLayer
    {
        BACK_TO_MAIN_MENU
    };

    UITableWidget* m_table{nullptr};

    UIMessage* m_popupMessage{nullptr};

    RotatingImage* m_connectionRotatingImage{nullptr};

    NextMultiplayerMenuLayer m_nextLayer;
    sf::RenderWindow& m_window;
    Scene m_scene;
};

#endif //MULTIPLAYER_MENU_LAYER_HPP