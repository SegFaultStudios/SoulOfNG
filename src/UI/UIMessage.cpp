#include "UI/UIMessage.hpp"

UIMessage::UIMessage(const std::string& name, UIWidget* parent) : UIWidget(name, parent), m_text("")
{
    m_background.setFillColor(sf::Color(0, 0, 0, 180));
    m_background.setSize({300, 60});

    m_border.setSize({m_background.getGlobalBounds().size.x, m_background.getGlobalBounds().size.y});
    m_border.setPosition({m_background.getGlobalBounds().position.x, m_background.getGlobalBounds().position.y});

    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineThickness(2);
    m_border.setOutlineColor(sf::Color::White);

    m_text.setTextColor(sf::Color::White);

    hide();
}

void UIMessage::resizeToFitText()
{
    sf::FloatRect bounds = m_text.getRawText()->getLocalBounds();

    float width = bounds.size.x + m_padding * 2;
    float height = bounds.size.y + m_padding * 2;

    setSize({width, height});
}

void UIMessage::update(float deltaTime)
{
    if (m_duration > 0.f)
    {
        m_timer += deltaTime;

        if (m_timer >= m_duration)
        {
            setVisible(false);
            m_duration = 0.f;
        }
    }

    sf::Vector2f pos = getPosition();
    m_background.setPosition(pos);

    m_text.setPosition({pos.x + 10.f, pos.y + 10.f});
}

void UIMessage::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);

    m_background.setPosition(position);

    m_text.setPosition({
        position.x + m_padding,
        position.y + m_padding
    });

    m_border.setSize({m_background.getGlobalBounds().size.x, m_background.getGlobalBounds().size.y});
    m_border.setPosition({m_background.getGlobalBounds().position.x, m_background.getGlobalBounds().position.y});
}

void UIMessage::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);

    m_background.setSize(size);

    sf::Vector2f position = getPosition();

    m_text.setPosition({
        position.x + m_padding,
        position.y + m_padding
    });

    m_border.setSize({m_background.getGlobalBounds().size.x, m_background.getGlobalBounds().size.y});
    m_border.setPosition({m_background.getGlobalBounds().position.x, m_background.getGlobalBounds().position.y});
}

void UIMessage::showFor(float seconds)
{
    m_duration = seconds;
    m_timer = 0.f;
    setVisible(true);
}

void UIMessage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!isVisible())
        return;

    m_text.draw(target, states);
    target.draw(m_background, states);
    target.draw(m_border, states);
}   

void UIMessage::setText(const std::string& text)
{
    m_text.setText(text);
    resizeToFitText();
}

UIText& UIMessage::getRawText()
{
    return m_text;
}