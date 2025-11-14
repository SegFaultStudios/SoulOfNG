#ifndef ROTATION_IMAGE_UI_HPP
#define ROTATION_IMAGE_UI_HPP

#include "UI/UIWidget.hpp"
#include "AssetsManager.hpp"
#include <memory>

class RotatingImage : public UIWidget
{
public:
    explicit RotatingImage(const std::string& name, sf::Texture* texture);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float deltaTime) override;

    void setPosition(const sf::Vector2f& position) override;

    void setSize(const sf::Vector2f& size) override;
    
    void stopSpinning();

    void startSpinning();
private:
    std::unique_ptr<sf::Sprite> m_sprite{nullptr};
    float m_rotationSpeed{180.0f};
    bool m_shoudSpin{true};
};

#endif //ROTATION_IMAGE_UI_HPP