#include "Widgets/LobbyWidget.hpp"

LobbyWidget::LobbyWidget(const std::string& name, UIWidget* parent) : UIWidget(name, parent)
{
    m_chatBackground.setFillColor(sf::Color(20, 20, 20, 180));
}

void LobbyWidget::addPlayer(const std::string& username)
{
    PlayerRow row;
    row.bg.setFillColor(sf::Color(60, 60, 60, 160));

    row.name = std::make_unique<UIText>("");
    row.name->setText(username);
    row.name->setCharacterSize(20);

    m_players.push_back(std::move(row));
}

void LobbyWidget::removePlayer(std::size_t index)
{
    if (index < m_players.size())
        m_players.erase(m_players.begin() + index); 
}

void LobbyWidget::clearPlayers()
{
    m_players.clear();
}

void LobbyWidget::setRowHeight(float height)
{
    m_rowHeight = height;
}

void LobbyWidget::setSpacing(float spacing)
{
    m_spacing = spacing;
}

void LobbyWidget::addChatMessage(const std::string& message)
{
    auto msg = std::make_unique<UIText>("");
    msg->setText(message);
    msg->setCharacterSize(16);
    m_chatMessages.push_back(std::move(msg));
}

void LobbyWidget::update(float dt)
{
    UIWidget::update(dt);

    float x = getPosition().x;
    float y = getPosition().y;

    for (auto& row : m_players)
    {
        row.bg.setPosition({x, y});
        row.bg.setSize({getSize().x * 0.6f, m_rowHeight});

        row.bounds = sf::FloatRect(row.bg.getPosition(), row.bg.getSize());

        row.name->setPosition({x + 12.f, y + 8.f});

        y += m_rowHeight + m_spacing;
    }

    float chatY = getPosition().y;
    float chatX = getPosition().x + getSize().x * 0.62f;
    float chatW = getSize().x * 0.36f;
    float chatH = getSize().y;

    m_chatBackground.setPosition({chatX, chatY});
    m_chatBackground.setSize({chatW, chatH});

    float cy = chatY + 10.f;
    for (auto& line : m_chatMessages)
    {
        line->setPosition({chatX + 10.f, cy});
        cy += m_chatLineHeight;
    }
}

void LobbyWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    UIWidget::handleEvent(event, window);
}

void LobbyWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!isVisible())
        return;

    for (auto& p : m_players)
    {
        target.draw(p.bg, states);
        p.name->draw(target, states);
    }

    target.draw(m_chatBackground, states);
    for (auto& msg : m_chatMessages)
        msg->draw(target, states);
}