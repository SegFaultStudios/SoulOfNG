#include <SFML/Window.hpp>
#include <optional>
#include "Player.hpp"
#include "Scene.hpp"

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Hello NG", sf::Style::Default);

    sf::View view = window.getDefaultView();

    Scene scene;

    scene.addEntity<Player>("Player");

    sf::Clock clock;
    float deltaTime = 0.0f;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* resized = event->getIf<sf::Event::Resized>()) 
            {
                sf::Vector2f newSize(resized->size.x, resized->size.y);
                view.setSize(newSize);

                window.setView(view);
            }

            scene.handleInput(event.value());
            std::cout << "scene.handleInput" << std::endl;
        }

        scene.update(deltaTime);

        window.clear();

        scene.draw(window);

        window.display();

        // std::cout << "Frame time: " << deltaTime * 1000.0f << "ms" << std::endl; //How much time took to render a frame
        // std::cout << "FPS: " << 1.0f / deltaTime << std::endl; //FPS counter
    }
}