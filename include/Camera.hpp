#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Camera
{
public:
    Camera(sf::RenderWindow& window);

    //Set target which camera follows
    void setTarget(Entity* target);

    void update(float deltaTime);
private:
    sf::RenderWindow& m_window;
    sf::View m_view;

    float m_zoom{1.0f};
    float m_smoothness{5.0f};

    Entity* m_targetEntity{nullptr};
};

#endif //CAMERA_HPP