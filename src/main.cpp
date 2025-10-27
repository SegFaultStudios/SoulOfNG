#include <SFML/Window.hpp>
#include <optional>

int main()
{
    sf::Window window(sf::VideoMode({800, 600}), "Hello NG");

    while (window.isOpen())
    {
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}