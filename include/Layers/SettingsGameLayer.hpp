#ifndef SETTINGS_GAME_LAYER_HPP
#define SETTINGS_GAME_LAYER_HPP

#include "Layers/Layer.hpp"
#include "Scene.hpp"

#include "UI/UICheckBox.hpp"
#include "UI/UIButton.hpp"

class SettingsGameLayer : public Layer
{
public:
    SettingsGameLayer(sf::RenderWindow& window);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;
    void onStart() override;
    void onEnd() override;
    std::unique_ptr<Layer> getNextLayer() const override;
private:
    Scene m_scene;
    sf::RenderWindow& m_window;
    UICheckBox* m_fullscreenCheckBox{nullptr};
    UIButton* m_backButton{nullptr};
};

#endif //SETTINGS_GAME_LAYER_HPP