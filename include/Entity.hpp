#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "AssetsManager.hpp"

enum class EntityEnum : uint8_t
{
    ENTITY = 0,
    PLAYER = 1,
    WALL = 2
};

class Entity
{
public:
    using SharedPtr = std::shared_ptr<Entity>;
    using UniquePtr = std::unique_ptr<Entity>;

    explicit Entity(const std::string& name) : m_name(name)
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

    virtual void handleInput(const sf::Event& event) 
    {

    }

    virtual void update(float deltaTime)
    {

    }

    void setTexture(const sf::Texture& texture)
    {
        sf::Vector2f currentScale = getScale();
        
        sprite_->setTexture(texture, true);

        setScale(currentScale);
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

    virtual void setOrigin(const sf::Vector2f& origin)
    {
        m_origin = origin;
        sprite_->setOrigin(origin);
    }

    const sf::Vector2f& getScale() const
    {
        return m_scale;
    }

    sf::FloatRect getGlobalBounds() 
    {
        return sprite_->getGlobalBounds();
    }
private:
    sf::Vector2f m_position{0.0f, 0.0f};
    sf::Vector2f m_scale{0.5f, 0.5f};
    sf::Vector2f m_origin{0.0f, 0.0f};
    //TODO later
    // sf::Vector2f m_rotation; 

    std::string m_name;

protected:
    std::unique_ptr<sf::Sprite> sprite_{nullptr};
};

#endif //ENTITY_HPP