#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <memory>

#include "Entities/Player.hpp"
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

    void cleanup();
private:
    sf::RenderWindow m_window;
    std::unique_ptr<Layer> m_layer{nullptr};
};

#endif //GAME_HPP