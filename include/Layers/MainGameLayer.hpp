#ifndef MAIN_GAME_LAYER_HPP
#define MAIN_GAME_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Room.hpp"
#include "BSP/BSPGenerator.hpp"
#include "QuadTree.hpp"
#include "AssetsManager.hpp"
#include "Widgets/EscapeUI.hpp"

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
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    EscapeUI* m_escapeUI{nullptr};
    sf::VertexArray m_rayLine;
    sf::RenderWindow& m_window;
    Scene m_scene;
    BSPGenerator m_BSPGenerator{sf::Vector2f(50, 50), 3,
        sf::FloatRect(sf::Vector2f(0, 0),
            sf::Vector2f(500, 500))};




    std::unique_ptr<Camera> m_camera{nullptr};
#if USE_EDITOR
    std::unique_ptr<Editor> m_editor{nullptr};
#endif
};

#endif //MAIN_GAME_LAYER_HPP