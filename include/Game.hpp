#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <memory>

#include "Player.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "AssetsManager.hpp"

#include "Layers/Layer.hpp"

#if USE_EDITOR
    #include "Editor.hpp"
#endif 

class Game
{
public:
    Game(const std::string& gameName);

    void run();
private:
    sf::RenderWindow m_window;
    Scene m_scene;
    std::unique_ptr<Camera> m_camera{nullptr};
#if USE_EDITOR
    std::unique_ptr<Editor> m_editor{nullptr};
#endif

    std::vector<std::unique_ptr<Layer>> m_layers;

    std::vector<std::unique_ptr<Layer>>::iterator m_currentLayer;
};

#endif //GAME_HPP