#include "Camera.hpp"

#include <cmath>

Camera::Camera(sf::RenderWindow& window) : m_window(window), m_view(window.getDefaultView())
{

}

void Camera::handleEvent(const sf::Event& event)
{
    if (auto* resized = event.getIf<sf::Event::Resized>()) 
    {
        sf::Vector2f newSize(resized->size.x, resized->size.y);
        m_view.setSize(newSize);
    }
}

const sf::View& Camera::getView() const
{
    return m_view;
}

Entity* Camera::getTarget() const
{
    return m_targetEntity;
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
    sf::Vector2f targetPosition = m_targetEntity->getPosition();
    float alpha = 1.0f - std::exp(-m_smoothness * deltaTime);

    sf::Vector2f newCenter = currentCenter + (targetPosition - currentCenter) * alpha;

    sf::Vector2f viewSize = sf::Vector2f(m_window.getSize()) * m_zoom;

    newCenter.x = currentCenter.x + (targetPosition.x - currentCenter.x) * alpha;
    newCenter.y = currentCenter.y + (targetPosition.y - currentCenter.y) * alpha;

    m_view.setSize(viewSize);
    m_view.setCenter(newCenter);

    m_window.setView(m_view);
}