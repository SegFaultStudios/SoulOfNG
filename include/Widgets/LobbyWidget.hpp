#ifndef LOBBY_WIDGET_HPP
#define LOBBY_WIDGET_HPP

#include "UI/UIWidget.hpp"
#include "UI/UIText.hpp"
#include <vector>
#include <string>

class LobbyWidget : public UIWidget
{
public:
    explicit LobbyWidget(const std::string& name, UIWidget* parent = nullptr);

    void addPlayer(const std::string& username);
    void removePlayer(std::size_t index);
    void clearPlayers();

    void setRowHeight(float height);
    void setSpacing(float spacing);

    void addChatMessage(const std::string& message);

    void update(float dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:    
    struct PlayerRow 
    {
        sf::RectangleShape bg;
        std::unique_ptr<UIText> name;
        sf::FloatRect bounds;
    };

    float m_rowHeight = 40.f;
    float m_spacing = 6.f;

    std::vector<PlayerRow> m_players;

    std::vector<std::unique_ptr<UIText>> m_chatMessages;
    float m_chatLineHeight = 18.f;
    sf::RectangleShape m_chatBackground;
};

#endif //LOBBY_WIDGET_HPP