#include "UI/UIButton.hpp"
#include <iostream>

#include "AssetsManager.hpp"

UIButton::UIButton(const std::string& name, const sf::Vector2f& size) : UIWidget(name)
{
    m_sprite = std::make_unique<sf::Sprite>(*AssetsManager::instance().getTexture("default"));

    if(!m_font.openFromFile("./resources/fonts/JetBrainsMono-Bold.ttf"))
    {
        std::cerr << "Failed to init font" << std::endl;
    }

    setSize(size);
    setPosition({400, 400});

    setOnHover([]{std::cout << "On hover\n";});
    setOnClick([]{std::cout << "On click\n";});
    setOnLeave([]{std::cout << "On leave\n";});
}

UIButton::UIButton(const std::string& name) : UIWidget(name)
{
    m_sprite = std::make_unique<sf::Sprite>(*AssetsManager::instance().getTexture("default"));

    if(!m_font.openFromFile("./resources/fonts/JetBrainsMono-Bold.ttf"))
    {
        std::cerr << "Failed to init font" << std::endl;
        //?Throw
    }

    m_text = std::make_unique<sf::Text>(m_font);
    m_text->setCharacterSize(16);
    m_text->setFillColor(sf::Color::Black);
    m_text->setStyle(sf::Text::Bold);

    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineThickness(2);
    m_border.setOutlineColor(sf::Color{50, 50, 50});

    setPosition({400, 400});
    setSize({0.4, 0.4});

    setOnHover([]{std::cout << "On hover\n";});
    setOnClick([]{std::cout << "On click\n";});
    setOnLeave([]{std::cout << "On leave\n";});
}

void UIButton::setTextCharacterSize(int size)
{
    m_text->setCharacterSize(size);
    updateTextPosition();
}

void UIButton::setTexturedColor(const sf::Color& color)
{
    sf::Image image({getSize().x, getSize().y}, color);

    sf::Texture texture;

    if(!texture.loadFromImage(image))
    {
        std::cerr << "failed to create textured color" << std::endl;
        return;
    }

    //Yeah that is weird
    auto createdTexture = AssetsManager::instance().addTexture(texture, "default_" 
    + std::to_string(color.r) + "_"
    + std::to_string(color.g) + "_"
    + std::to_string(color.b)); 

    m_sprite->setTexture(*createdTexture);
}

void UIButton::setText(const std::string& text)
{
    m_text->setString(text);
    updateTextPosition();
}

void UIButton::setTextColor(const sf::Color& color)
{
    m_text->setFillColor(color);
}

void UIButton::updateTextPosition()
{
    if (!m_sprite) 
        return;
    
    sf::FloatRect buttonBounds = m_sprite->getGlobalBounds();
    sf::FloatRect textBounds = m_text->getLocalBounds();

    float textX = buttonBounds.position.x + (buttonBounds.size.x - textBounds.size.x) / 2.0f;
    float textY = buttonBounds.position.y + (buttonBounds.size.y - textBounds.size.y) / 2.0f;    

    textY -= textBounds.position.y;
    
    m_text->setPosition({textX, textY});
}

void UIButton::setTexture(const sf::Texture& texture)
{
    m_sprite->setTexture(texture);
}

const sf::Texture& UIButton::getTexture() const
{   
    return m_sprite->getTexture();
}

void UIButton::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_sprite->setPosition(position);
    updateTextPosition();
    m_border.setSize({m_sprite->getGlobalBounds().size.x, m_sprite->getGlobalBounds().size.y});
    m_border.setPosition({m_sprite->getGlobalBounds().position.x, m_sprite->getGlobalBounds().position.y});
}   

void UIButton::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
    m_sprite->setScale(size);
    updateTextPosition();
    m_border.setSize({m_sprite->getGlobalBounds().size.x, m_sprite->getGlobalBounds().size.y});
    m_border.setPosition({m_sprite->getGlobalBounds().position.x, m_sprite->getGlobalBounds().position.y});
}

void UIButton::setBorderColor(const sf::Color& color)
{
    m_border.setOutlineColor(color);
}

void UIButton::handleCustomEvent(CustomEvent event)
{
    // switch (event)
    // {
    //     case CustomEvent::HOVERED:
    //         m_text->setFillColor(sf::Color::Yellow);
    //         break;
            
    //     case CustomEvent::LEAVE:
    //         m_text->setFillColor(sf::Color::White);
    //         break;
            
    //     case CustomEvent::CLICKED:
    //         m_text->setFillColor(sf::Color::Green);
    //         break;
            
    //     default:
    //         break;
    // }
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_sprite.get(), states);
    target.draw(*m_text.get(), states);
    target.draw(m_border, states);
}

sf::FloatRect UIButton::getBoundingBox() const
{
    return m_sprite->getGlobalBounds();
}