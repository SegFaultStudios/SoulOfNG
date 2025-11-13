#ifndef MAIN_GAME_LAYER_HPP
#define MAIN_GAME_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Room.hpp"
#include "BSP/BSPGenerator.hpp"
#include "QuadTree.hpp"
#include "AssetsManager.hpp"

#if USE_EDITOR
    #include "Editor.hpp"
#endif 

class MainGameLayer : public Layer
{
public:
    MainGameLayer(sf::RenderWindow& window);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;


private:
    sf::VertexArray m_rayLine;
    sf::RenderWindow& m_window;
    Scene m_scene;
    BSPGenerator m_BSPGenerator{sf::Vector2f(100, 100), 4,
        sf::FloatRect(sf::Vector2f(0, 0),
            sf::Vector2f(1000, 1000))};




    std::unique_ptr<Camera> m_camera{nullptr};
#if USE_EDITOR
    std::unique_ptr<Editor> m_editor{nullptr};
#endif
};

#endif //MAIN_GAME_LAYER_HPP