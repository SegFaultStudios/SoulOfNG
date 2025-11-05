#ifndef MAIN_MENU_LAYER_HPP
#define MAIN_MENU_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"

class MainMenuLayer : public Layer
{
public:
    MainMenuLayer(sf::RenderWindow& window);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;

private:
    Scene m_scene;
    sf::RenderWindow& m_window;
};

#endif //MAIN_MENU_LAYER_HPP