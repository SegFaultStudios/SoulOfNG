#include "Widgets/EscapeUI.hpp"

EscapeUI::EscapeUI(const std::string &name) : UIWidget(name)
{
    m_background.setFillColor(sf::Color(255, 255, 255, 120));
    m_background.setSize({200, 300});
    m_background.setPosition({400, 100});

    m_backToMainMenuButton = std::make_unique<UIButton>("");

    m_backToMainMenuButton->setText("Main menu");

    m_backToMainMenuButton->setSize({10, 5});
    m_backToMainMenuButton->setPosition({400, 80});
    m_backToMainMenuButton->setTextColor(sf::Color::White);
    m_backToMainMenuButton->setBorderColor(sf::Color::White);
    m_backToMainMenuButton->setTexturedColor(sf::Color{50, 50, 50});
}

UIButton *EscapeUI::getMainMenuButton() const
{
    return m_backToMainMenuButton.get();
}

void EscapeUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!isVisible())
        return;

    target.draw(m_background, states);
    m_backToMainMenuButton->draw(target, states);
}

sf::FloatRect EscapeUI::getBoundingBox() const
{
    return m_background.getGlobalBounds();
}

void EscapeUI::setPosition(const sf::Vector2f &position)
{
    UIWidget::setPosition(position);
}

void EscapeUI::setSize(const sf::Vector2f &size)
{
    UIWidget::setSize(size);
}

void EscapeUI::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (!isVisible())
        return;
    m_backToMainMenuButton->handleEvent(event, window);
}