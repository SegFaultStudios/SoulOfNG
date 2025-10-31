#include "UI/UIButton.hpp"
#include <iostream>

UIButton::UIButton(const std::string& name) : UIWidget(name)
{
    m_background.setFillColor(sf::Color::White);
    m_background.setSize({10, 10});
    m_background.setPosition({400, 400});

    setPosition({400, 400});

    setSize({10, 10});

    setOnHover([]{std::cout << "On hover\n";});
    setOnClick([]{std::cout << "On click\n";});
    setOnLeave([]{std::cout << "On leave\n";});
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
}

sf::FloatRect UIButton::getBoundingBox() const
{
    return m_background.getGlobalBounds();
}