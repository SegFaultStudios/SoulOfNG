#ifndef MULTIPLAYER_MENU_LAYER_HPP
#define MULTIPLAYER_MENU_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"
#include "Widgets/MultiplayerMenuConnectionDialogWidget.hpp"
#include "UI/UITableWidget.hpp"

class MultiplayerMenuLayer : public Layer
{
public:
    explicit MultiplayerMenuLayer(sf::RenderWindow& window);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    enum class NextMultiplayerMenuLayer
    {
        BACK_TO_MAIN_MENU
    };

    UITableWidget* m_table{nullptr};

    NextMultiplayerMenuLayer m_nextLayer;
    MultiplayerMenuConnectionDialogWidget* m_dialogWidget{nullptr};
    sf::RenderWindow& m_window;
    Scene m_scene;
};

#endif //MULTIPLAYER_MENU_LAYER_HPP