#include "UI/UIText.hpp"
#include <iostream>

UIText::UIText(const std::string& name) : UIWidget(name)
{
    if (!m_font.openFromFile("./resources/fonts/JetBrainsMono-Bold.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    m_text = std::make_unique<sf::Text>(m_font, name);
}

void UIText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!isVisible())
        return;
        
    target.draw(*m_text.get(), states);
}

void UIText::setTextColor(const sf::Color& color)
{
    m_text->setFillColor(color);
}

std::string UIText::getText() const
{
    return m_text->getString();
}

void UIText::setCharacterSize(unsigned int size)
{
    m_text->setCharacterSize(size);
}

void UIText::setText(const std::string& text)
{
    m_text->setString(text);
}

sf::FloatRect UIText::getBoundingBox() const
{
    return sf::FloatRect();
}

void UIText::handleCustomEvent(CustomEvent event)
{

}

sf::Text* UIText::getRawText() const
{
    return m_text.get();
}

unsigned int UIText::getCharacterSize() const
{
    return m_text->getCharacterSize();
}

void UIText::setPosition(const sf::Vector2f& position) 
{
    UIWidget::setPosition(position);
    m_text->setPosition(position);
}

void UIText::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
}