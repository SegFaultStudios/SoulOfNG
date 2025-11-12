#include "UI/UIWidget.hpp"
#include <iostream>

UIWidget::UIWidget(const std::string& name) : m_name(name)
{

}

void UIWidget::setName(const std::string& name)
{
    m_name = name;
}

const std::string& UIWidget::getName() const
{
    return m_name;
}

void UIWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if(!m_isVisible)
        return;

    if(auto mouseEvent = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        // std::cout << "Mouse position: X: " << mousePosition.x << " Y: " << mousePosition.y << std::endl;

        // auto mouse = window.mapCoordsToPixel({mousePosition.x, mousePosition.y});

        // std::cout << "Converted mouse position X: " << mouse.x << " Y: " << mousePosition.y << std::endl;

        // std::cout << "Position: X: " << this->getPosition().x << " Y: " << this->getPosition().y << std::endl;

        bool isHovered = getBoundingBox().contains(sf::Vector2f{static_cast<float>(mousePosition.x), 
        static_cast<float>(mousePosition.y)});

        if(isHovered && !m_isHovered)
        {
            handleCustomEvent(CustomEvent::HOVERED);

            if(m_onHover)
                m_onHover();
        }
        else if(!isHovered && m_isHovered)
        {
            handleCustomEvent(CustomEvent::LEAVE);

            if(m_onLeave)
                m_onLeave();
        }

        m_isHovered = isHovered;
    }
    else if(auto mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(mouseClick->button == sf::Mouse::Button::Left && m_isHovered)
        {   
            handleCustomEvent(CustomEvent::CLICKED);

            if(m_onClick)
                m_onClick();
        }
    }
}

bool UIWidget::isVisible() const
{
    return m_isVisible;
}

void UIWidget::update(float deltaTime)
{

}

const sf::Vector2f& UIWidget::getPosition() const
{
    return m_position;
}

const sf::Vector2f& UIWidget::getSize() const
{
    return m_size;
}

void UIWidget::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}

void UIWidget::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

void UIWidget::hide()
{
    setVisible(false);
}

void UIWidget::show()
{
    setVisible(true);
}

void UIWidget::setVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

void UIWidget::setOnClick(const EventCallback& callback)
{
    m_onClick = callback;
}

void UIWidget::setOnHover(const EventCallback& callback)
{
    m_onHover = callback;
}

void UIWidget::setOnLeave(const EventCallback& callback)
{
    m_onLeave = callback;
}

UIWidget::~UIWidget() = default;