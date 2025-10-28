#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

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
    virtual void draw(sf::RenderWindow& target) = 0;
    virtual ~Entity() = default;

    virtual void setPosition(const sf::Vector2f& position)
    {
        m_position = position;
    }

    const sf::Vector2f& getPosition() const
    {
        return m_position;
    }

    virtual void setScale(const sf::Vector2f& scale)
    {
        m_scale = scale;
    }

    const sf::Vector2f& getScale() const
    {
        return m_scale;
    }

private:
    sf::Vector2f m_position;
    sf::Vector2f m_scale;

    //TODO later
    // sf::Vector2f m_rotation; 

    std::string m_name;
};

#endif //ENTITY_HPP