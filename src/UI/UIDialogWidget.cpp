#include "UI/UIDialogWidget.hpp"


UIDialogWidget::UIDialogWidget(const std::string& name)
    : UIWidget(name), m_titleText("")
{
    m_background.setFillColor(sf::Color(40, 40, 40, 220));
    m_background.setOutlineThickness(2.f);
    m_background.setOutlineColor(sf::Color::White);

    m_titleBar.setFillColor(sf::Color(30, 30, 30, 255));

    m_titleText.setCharacterSize(18);
}

void UIDialogWidget::setTitle(const std::string& title)
{
    m_titleText.setText(title);
}

void UIDialogWidget::open()
{
    m_open = true;
    m_result = DialogResult::None;
}

void UIDialogWidget::close()
{
    m_open = false;
}

void UIDialogWidget::setResult(DialogResult result)
{
    m_result = result;
}

UIDialogWidget::DialogResult UIDialogWidget::exec(sf::RenderWindow& window)
{
    open();

    while (m_open)
    {
        auto event = window.pollEvent();

        if(event.has_value())
            this->handleEvent(event.value(), window);

        window.draw(*this);
    }

    return m_result;
}

bool UIDialogWidget::isOpen() const
{
    return m_open;
}

UIDialogWidget::DialogResult UIDialogWidget::getResult() const
{
    return m_result;
}

void UIDialogWidget::setPosition(const sf::Vector2f& position)
{
    UIWidget::setPosition(position);
    m_background.setPosition(position);
    m_titleBar.setPosition(position);

    m_titleText.setPosition({position.x + 10.f, position.y + 5.f});
}

void UIDialogWidget::setSize(const sf::Vector2f& s)
{
    UIWidget::setSize(s);
    m_background.setSize(s);

    m_titleBar.setSize({s.x, 30.f});
}

void UIDialogWidget::update(float deltaTime)
{
    if (!m_open)
        return;

    UIWidget::update(deltaTime);
}

void UIDialogWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_open)
        return;

    target.draw(m_background, states);
    target.draw(m_titleBar, states);
    m_titleText.draw(target, states);
}

void UIDialogWidget::beginDrag(const sf::Vector2f& mouse)
{
    m_dragging = true;
    m_dragOffset = mouse - getPosition();
}

void UIDialogWidget::updateDrag(const sf::Vector2f& mouse)
{
    if (!m_dragging)
        return;

    setPosition(mouse - m_dragOffset);
}

void UIDialogWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (!m_open)
        return;

    UIWidget::handleEvent(event, window);

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (auto* press = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (press->button == sf::Mouse::Button::Left)
        {
            if (m_titleBar.getGlobalBounds().contains(mouse))
                beginDrag(mouse);
        }
    }
    else if (auto* rel = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (rel->button == sf::Mouse::Button::Left)
            m_dragging = false;
    }
    else if (auto* move = event.getIf<sf::Event::MouseMoved>())
    {
        updateDrag(mouse);
    }
}