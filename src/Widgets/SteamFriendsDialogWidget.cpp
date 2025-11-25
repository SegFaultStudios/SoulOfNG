#include "Widgets/SteamFriendsDialogWidget.hpp"
#include "Logger.hpp"

SteamFriendsDialogWidget::SteamFriendsDialogWidget(const std::string& name, UIWidget* parent) : UIDialogWidget(name)
{
    this->setTitle("Steam friends");

    if (!m_font.openFromFile("./resources/fonts/JetBrainsMono-Bold.ttf"))
        LOG_WARNING("Failed to load font");

    m_backButton = std::make_unique<UIButton>("");

    m_backButton->setText("Back");
    m_backButton->setSize({5.0f, 3.0f});
    m_backButton->setPosition({getPosition() + sf::Vector2f{50.0f, 500.0f}});
    m_backButton->setTexturedColor(sf::Color(50, 50, 50));

    HANDLE_EVENT(m_backButton, UIButton::clicked, this, [this]
    {
        this->setResult(DialogResult::Rejected);
        this->close();
    });
}

void SteamFriendsDialogWidget::removeAllFriends()
{
    m_friends.clear();
}

void SteamFriendsDialogWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    UIDialogWidget::draw(target, states);

    if(!isOpen())
        return;

    constexpr float titleBarHeight = 30.f;
    sf::Vector2f base = getPosition();
    base.y += titleBarHeight;

    float y = m_padding;
    
    for (const auto& f : m_friends)
    {
        if (f.avatar.getSize().x > 0)
        {
            sf::Sprite spr(f.avatar);
            spr.setPosition(base + sf::Vector2f{m_padding, y});
            spr.setScale({32.f / f.avatar.getSize().x, 32.f / f.avatar.getSize().y});
            target.draw(spr, states);
        }

        float nameMaxWidth = getSize().x - (m_padding + 40 + 60);
        std::string displayName = truncateToWidth(f.name, nameMaxWidth);        

        sf::Text text(m_font);
        text.setCharacterSize(18);
        text.setString(displayName);
        text.setFillColor(sf::Color::White);
        text.setPosition(base + sf::Vector2f{m_padding + 40, y + 4});
        target.draw(text, states);

        UIButton inviteButton("");
        inviteButton.setTexturedColor(sf::Color{50, 50, 50});
        inviteButton.setPosition(getPosition() + sf::Vector2f{getSize().x - 100.f - m_padding, y + 25.0f});
        inviteButton.setText("Invite");
        inviteButton.setSize({5.f, 1.5f});
        inviteButton.draw(target, states);

        f.inviteBtnRect = inviteButton.getBoundingBox();

        y += 40;
    }

    m_backButton->draw(target, states);
}

void SteamFriendsDialogWidget::addFriend(const std::string& name, const sf::Texture& texture, CSteamID id)
{
    FriendEntry friendEntry;
    friendEntry.avatar = texture;
    friendEntry.name = name;
    friendEntry.id = id;
    m_friends.push_back(std::move(friendEntry));
}

void SteamFriendsDialogWidget::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    UIDialogWidget::handleEvent(event, window);

    if(!isOpen())
        return;

    m_backButton->handleEvent(event, window);

    if (auto* m = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (m->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            for (auto& f : m_friends)
            {
                if (f.inviteBtnRect.contains(mp))
                {
                    LOG_INFO("Invited");
                    CALL_SIGNAL(inviteFriend, f.id);
                    break;
                }
            }
        }
    }
}

void SteamFriendsDialogWidget::setPosition(const sf::Vector2f &position)
{
    UIDialogWidget::setPosition(position);

    m_backButton->setPosition(getPosition() + sf::Vector2f{50.0f, 500.0f});
}

void SteamFriendsDialogWidget::update(float deltaTime)
{
    UIDialogWidget::update(deltaTime);

    if(!isOpen())
        return;

    m_backButton->update(deltaTime);
}
