#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Scene.hpp"

class Editor
{
public:
    Editor(sf::RenderWindow& window, Scene& scene);

    void processEvents(sf::Event& event);

    void update(sf::Time deltaTime);

    void draw();

private:
    void drawHierarchy();
    void drawSelectedEntityDetails();
    void drawAddManager();

    sf::RenderWindow& m_window;
    Scene& m_scene;

    uint64_t m_selectedEntityId;
};


#endif //EDITOR_HPP