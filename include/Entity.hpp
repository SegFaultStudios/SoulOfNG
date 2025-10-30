#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "AssetsManager.hpp"

enum class EntityEnum : uint8_t
{
    NONE = 0,
    PLAYER = 1
};

class Entity
{
public:
    using SharedPtr = std::shared_ptr<Entity>;
    using UniquePtr = std::unique_ptr<Entity>;

    Entity(const std::string& name) : m_name(name)
    {
        //Setting default white texture
        sprite_ = std::make_unique<sf::Sprite>(*AssetsManager::instance().getTexture("default"));
    }

    void setName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& getName() const
    {
        return m_name;
    }

    virtual void handleInput(sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;

    void setTexture(const sf::Texture& texture)
    {
        sprite_->setTexture(texture);
    }

    const sf::Texture& getTexture() const
    {
        return sprite_->getTexture();
    }

    virtual void draw(sf::RenderWindow& target)
    {
        target.draw(*sprite_);
    }

    virtual ~Entity() = default;

    virtual void setPosition(const sf::Vector2f& position)
    {
        m_position = position;
        sprite_->setPosition(position);
    }

    const sf::Vector2f& getPosition() const
    {
        return m_position;
    }

    virtual void setScale(const sf::Vector2f& scale)
    {
        m_scale = scale;
        sprite_->setScale(scale);
    }

    const sf::Vector2f& getScale() const
    {
        return m_scale;
    }
private:
    sf::Vector2f m_position{0.0f, 0.0f};
    sf::Vector2f m_scale{0.5f, 0.5f};
    //TODO later
    // sf::Vector2f m_rotation; 

    std::string m_name;

protected:
    std::unique_ptr<sf::Sprite> sprite_{nullptr};
};

#endif //ENTITY_HPP