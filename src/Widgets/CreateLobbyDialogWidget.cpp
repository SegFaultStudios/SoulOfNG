#include "Widgets/CreateLobbyDialogWidget.hpp"

CreateLobbyDialogWidget::CreateLobbyDialogWidget(const std::string &name, UIWidget *parent) : UIDialogWidget(name),
m_lobbyNameInputText("")
{
    setTitle("Create lobby");

    sf::Vector2f buttonSize{5.0f, 2.5f};

    m_createLobbyButton = std::make_unique<UIButton>("");
    m_backButton = std::make_unique<UIButton>("");

    m_lobbyNameInputText.setPosition(getPosition() + sf::Vector2f(150, 100));
    m_lobbyNameInputText.setSize({250.0f, 50.0f});

    m_backButton->setPosition(getPosition() + sf::Vector2f(50, 400));
    m_backButton->setSize(buttonSize);
    m_backButton->setTexturedColor(sf::Color{50, 50, 50});
    m_backButton->setText("Back");

    HANDLE_EVENT(m_backButton, UIButton::clicked, this, [this]
    {
        this->setResult(DialogResult::Rejected);
        this->close();
    });

    m_createLobbyButton->setPosition(getPosition() + sf::Vector2f(400, 400));
    m_createLobbyButton->setSize(buttonSize);
    m_createLobbyButton->setTexturedColor(sf::Color{50, 50, 50});
    m_createLobbyButton->setText("Create");

    HANDLE_EVENT(m_createLobbyButton, UIButton::clicked, this, [this]
    {
        this->setResult(DialogResult::Accepted);
        //Let layer close it since we need acception from server
    });
}

std::string CreateLobbyDialogWidget::getLobbyName()
{   
    return m_lobbyNameInputText.getText().getText();
}

void CreateLobbyDialogWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    UIDialogWidget::draw(target, states);

    if (!isOpen())
        return;

    m_lobbyNameInputText.draw(target, states);
    m_backButton->draw(target, states);
    m_createLobbyButton->draw(target, states);
}

void CreateLobbyDialogWidget::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    UIDialogWidget::handleEvent(event, window);

    if (!isOpen())
        return;

    m_backButton->handleEvent(event, window);
    m_createLobbyButton->handleEvent(event, window);
    m_lobbyNameInputText.handleEvent(event, window);
}

void CreateLobbyDialogWidget::setPosition(const sf::Vector2f &position)
{
    UIDialogWidget::setPosition(position);

    m_lobbyNameInputText.setPosition(getPosition() + sf::Vector2f(150, 100));
    m_backButton->setPosition(getPosition() + sf::Vector2f(50, 400));
    m_createLobbyButton->setPosition(getPosition() + sf::Vector2f(400, 400));
}

void CreateLobbyDialogWidget::update(float deltaTime)
{
    UIDialogWidget::update(deltaTime);
    m_lobbyNameInputText.update(deltaTime);
}