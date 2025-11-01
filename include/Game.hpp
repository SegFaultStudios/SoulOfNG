#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

#include "Player.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "AssetsManager.hpp"

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
};

#endif //GAME_HPP