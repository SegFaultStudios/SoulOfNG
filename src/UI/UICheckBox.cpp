#include "UI/UICheckBox.hpp"

UICheckBox::UICheckBox(const std::string& name) : UIWidget(name)
{
    m_box.setOutlineColor(sf::Color::White);
    m_box.setFillColor(sf::Color::Red);
}

void UICheckBox::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_box.setPosition(position);
}

void UICheckBox::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
    m_box.setSize(size);
}

void UICheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_box, states);
}

bool UICheckBox::isChecked() const
{
    return m_isChecked;
}

sf::FloatRect UICheckBox::getBoundingBox() const
{
    return m_box.getGlobalBounds();
}

void UICheckBox::setIsChecked(bool isChecked)
{
    m_isChecked = isChecked;

    if(m_isChecked)
        m_box.setFillColor(sf::Color::Green);
    else
        m_box.setFillColor(sf::Color::Red);
}

void UICheckBox::handleCustomEvent(CustomEvent event)
{
    switch(event)
    {
        case CustomEvent::CLICKED:
        {
            setIsChecked(!m_isChecked);
            break;
        }
    }
}