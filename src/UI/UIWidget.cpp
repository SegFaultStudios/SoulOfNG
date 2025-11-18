#include "UI/UIWidget.hpp"
#include <iostream>

UIWidget::UIWidget(const std::string& name, UIWidget* parent) : m_name(name), m_parent(parent)
{
    if(parent)
        parent->m_children.push_back(this);
}

void UIWidget::setName(const std::string& name)
{
    m_name = name;
}

const std::string& UIWidget::getName() const
{
    return m_name;
}

sf::FloatRect UIWidget::getBoundingBox() const 
{ 
    return sf::FloatRect(m_position, m_size); 
}

void UIWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if(!m_isVisible)
        return;

    if(auto mouseEvent = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
        
        bool isHovered = getBoundingBox().contains(mouseWorldPos);

        if(isHovered && !m_isHovered)
        {
            handleCustomEvent(CustomEvent::HOVERED);

            hovered.emit();
        }
        else if(!isHovered && m_isHovered)
        {
            handleCustomEvent(CustomEvent::LEAVE);

            m_wasHoveredBeforeClick = true;

            left.emit();
        }

        m_isHovered = isHovered;
    }
    else if(auto mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(mouseClick->button == sf::Mouse::Button::Left && m_isHovered)
        {   
            handleCustomEvent(CustomEvent::CLICKED);
            
            clicked.emit();
        }
        else if (mouseClick->button == sf::Mouse::Button::Left && !m_isHovered && m_wasHoveredBeforeClick)
        {
            handleCustomEvent(CustomEvent::LEAVE_CLICK);

            m_wasHoveredBeforeClick = false;
        }
    }
}

bool UIWidget::isVisible() const
{
    return m_isVisible;
}

void UIWidget::update(float deltaTime)
{
    if(!isVisible())
        return;
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

UIWidget::~UIWidget() = default;