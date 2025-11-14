#include "UI/UIInputText.hpp"

UIInputText::UIInputText(const std::string& name) : UIWidget(name), m_text("")
{
    m_background.setFillColor(sf::Color(255, 255, 255, 128));
    m_cursor.setFillColor(sf::Color::Black);
    m_cursor.setSize({2.f, m_text.getCharacterSize() * 0.8f});
}   

void UIInputText::update(float deltaTime)
{
    UIWidget::update(deltaTime);

    if(!m_isFocused)
        return;

    m_cursorTimer += deltaTime;

    if (m_cursorTimer > 0.5f)
    {
        m_cursorVisible = !m_cursorVisible;
        m_cursorTimer = 0.f;
    }

    float textWidth = m_text.getRawText()->getLocalBounds().size.x;

    sf::Vector2f pos = m_text.getPosition();
    pos.x += textWidth + 2.f;
    pos.y -= m_cursor.getSize().y * 0.002;

    m_cursor.setPosition(pos);
}

void UIInputText::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    UIWidget::handleEvent(event, window);

    if(auto pressEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if(pressEvent->code == sf::Keyboard::Key::Backspace && m_isFocused)
        {
            if(!m_text.getText().empty())
            {
                std::string text = m_text.getText();
                text.pop_back();
                m_text.setText(text);
            }
        }
    }

    if(auto textEvent = event.getIf<sf::Event::TextEntered>())
    {
        if(textEvent->unicode < 128 && textEvent->unicode != '\b' && textEvent->unicode != '\r' && m_isFocused)
        {
            std::string text = m_text.getText();

            text += static_cast<char>(textEvent->unicode);

            m_text.setText(text);
        }
    }
}

void UIInputText::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_background.setPosition(position);

    m_text.setPosition({
        position.x + 5.f,
        position.y + (m_background.getSize().y - m_text.getCharacterSize()) * 0.5f
    });
}

void UIInputText::setSize(const sf::Vector2f& size)
{
    UIWidget::setSize(size);
    m_text.setSize(size);
    m_background.setSize(size);

    m_text.setPosition({
        m_background.getPosition().x + 5.f,
        m_background.getPosition().y + (size.y - m_text.getCharacterSize()) * 0.5f
    });
}

void UIInputText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    m_text.draw(target, states);

    if(m_cursorVisible && m_isFocused)
        target.draw(m_cursor, states);
}

sf::FloatRect UIInputText::getBoundingBox() const
{
    return m_background.getGlobalBounds();
}

UIText& UIInputText::getText()
{
    return m_text;
}

void UIInputText::handleCustomEvent(CustomEvent event)
{
    switch(event)
    {
        case CustomEvent::CLICKED:
            m_isFocused = true;
            break;
        case CustomEvent::LEAVE_CLICK:
            m_isFocused = false;
            break;
        default:
            break;
    }
}