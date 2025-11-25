#ifndef LOBBY_LAYER_HPP
#define LOBBY_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"
#include "Network/Client.hpp"
#include "Widgets/LobbyWidget.hpp"
#include "Widgets/SteamFriendsDialogWidget.hpp"
#include "Network/Lobby.hpp"
#include "Network/SteamClientNetwork.hpp"

class LobbyLayer : public Layer
{
public:
    LobbyLayer(sf::RenderWindow& window, std::unique_ptr<SteamClientNetwork> client);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    enum class NextLayer
    {
        BACK_TO_MULTIPLAYER_MENU
    };

    NextLayer m_nextLayer;

    LobbyData m_currentLobby;

    LobbyWidget* m_lobbyWidget{nullptr};
    SteamFriendsDialogWidget* m_friendsWidget{nullptr};
    mutable std::unique_ptr<SteamClientNetwork> m_steamClientNetwork{nullptr};
    Scene m_scene;
    sf::RenderWindow& m_window;
};

#endif //LOBBY_LAYER_HPP