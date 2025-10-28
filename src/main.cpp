#include <SFML/Window.hpp>
#include <optional>
#include "Player.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Hello NG", sf::Style::Default);

    sf::View view = window.getDefaultView();

    Player player;

    while (window.isOpen())
    {
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
        }

        window.clear();

        player.draw(window);

        window.display();
    }
}