#include "Camera.hpp"

#include <cmath>

Camera::Camera(sf::RenderWindow& window) : m_window(window), m_view(window.getDefaultView())
{

}

void Camera::setTarget(Entity* target)
{
    m_targetEntity = target;
}

void Camera::update(float deltaTime)
{
    if(!m_targetEntity)
        return;
    
    sf::Vector2f currentCenter = m_view.getCenter();

    sf::Vector2f newCenter = currentCenter;

    //Add offset to it
    sf::Vector2f targetPosition = m_targetEntity->getPosition();

    float alpha = 1.0f - std::exp(-m_smoothness * deltaTime);

    newCenter.x = currentCenter.x + (targetPosition.x - currentCenter.x) * alpha;
    newCenter.y = currentCenter.y + (targetPosition.y - currentCenter.y) * alpha;

    m_view.setCenter(newCenter);

    m_view.setSize({m_window.getSize().x * m_zoom, m_window.getSize().y * m_zoom});

    m_window.setView(m_view);
}

