#include "Widgets/AuthorizationDialogWidget.hpp"

AuthorizationDialogWidget::AuthorizationDialogWidget(const std::string& name, UIWidget* parent) : UIDialogWidget(name), m_loginInputText("")
{
    this->setTitle("Authorization");

    sf::Vector2f buttonSize{5.0f, 2.5f};

    m_loginInputText.setPosition(getPosition() + sf::Vector2f{150.0f, 50.0f});
    m_loginInputText.setSize({250.0f, 50.0f});

    m_loginButton = std::make_unique<UIButton>("");

    m_loginButton->setPosition(getPosition() + sf::Vector2f(400, 400));
    m_loginButton->setSize(buttonSize);
    m_loginButton->setTexturedColor(sf::Color{50, 50, 50});
    m_loginButton->setText("Create");

    HANDLE_EVENT(m_loginButton, UIButton::clicked, this, [this]
    {
        this->setResult(DialogResult::Accepted);
        this->close();
    });
}

void AuthorizationDialogWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    UIDialogWidget::draw(target, states);

    if (!isOpen())
        return;

    m_loginInputText.draw(target, states);
    m_loginButton->draw(target, states);
}

void AuthorizationDialogWidget::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    UIDialogWidget::handleEvent(event, window);

    if (!isOpen())
        return;

    m_loginInputText.handleEvent(event, window);
    m_loginButton->handleEvent(event, window);
}

void AuthorizationDialogWidget::setPosition(const sf::Vector2f &position)
{
    UIDialogWidget::setPosition(position);
    m_loginInputText.setPosition(getPosition() + sf::Vector2f{150.0f, 50.0f});
    m_loginButton->setPosition(getPosition() + sf::Vector2f(400, 400));
}

std::string AuthorizationDialogWidget::getText()
{
    return m_loginInputText.getText().getText();
}

void AuthorizationDialogWidget::update(float deltaTime)
{
    UIDialogWidget::update(deltaTime);

    if (!isOpen())
        return;

    m_loginInputText.update(deltaTime);
    m_loginButton->update(deltaTime);
}