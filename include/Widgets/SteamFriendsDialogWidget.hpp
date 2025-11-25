#ifndef STEAM_FRIENDS_DIALOG_WIDGET_HPP
#define STEAM_FRIENDS_DIALOG_WIDGET_HPP

#include "UI/UIDialogWidget.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"

#include "steam/steam_api.h"

#include "Signal.hpp"

class SteamFriendsDialogWidget : public UIDialogWidget
{
public:
    SteamFriendsDialogWidget(const std::string& name, UIWidget* parent = nullptr);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    void setPosition(const sf::Vector2f &position) override;

    void update(float deltaTime) override;

    void addFriend(const std::string& name, const sf::Texture& texture, CSteamID id);

    void removeAllFriends();

SIGNAL_NAMESPACE_BEGIN
    SIGNAL(inviteFriend, CSteamID);
SIGNAL_NAMESPACE_END

private:
    struct FriendEntry
    {
        std::string name;
        sf::Texture avatar;
        mutable sf::FloatRect inviteBtnRect;
        CSteamID id;
    };

    std::vector<FriendEntry> m_friends;

    float m_padding = 10.f;

    sf::Font m_font;

    UIButton::UniquePtr m_backButton{nullptr};

    std::string truncateToWidth(const std::string& s, float maxWidth) const
    {
        sf::Text txt(m_font);
        txt.setCharacterSize(18);

        std::string out;

        for (char c : s)
        {
            out.push_back(c);
            txt.setString(out);
            if (txt.getLocalBounds().position.x > maxWidth)
            {
                if (out.size() > 1)
                    out.pop_back();
                return out + "...";
            }
        }
        return out;
    }

};

#endif //STEAM_FRIENDS_DIALOG_WIDGET_HPP