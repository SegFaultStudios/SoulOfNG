#include "UI/UIButton.hpp"
#include <iostream>

UIButton::UIButton(const std::string& name) : UIWidget(name)
{
    m_background.setFillColor(sf::Color::White);
    
    setPosition({400, 400});
    setSize({10, 10});

    setOnHover([]{std::cout << "On hover\n";});
    setOnClick([]{std::cout << "On click\n";});
    setOnLeave([]{std::cout << "On leave\n";});
}

void UIButton::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_background.setPosition(position);
}   

void UIButton::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
    m_background.setSize(size);
}

void UIButton::handleCustomEvent(CustomEvent event)
{
    if(event == CustomEvent::HOVERED)
        m_background.setFillColor(m_hoverColor); 
    else if(event == CustomEvent::LEAVE)
        m_background.setFillColor(m_idleColor);
    else if(event == CustomEvent::CLICKED)
        m_background.setFillColor(m_clickedColor);
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
}

sf::FloatRect UIButton::getBoundingBox() const
{
    return m_background.getGlobalBounds();
}