#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"

#include <memory>
#include <unordered_map>

class Scene
{
public:
    template<typename T, typename... Args>
    uint64_t addEntity(const std::string& entityName, Args&&... args)
    {
        static_assert(!std::is_abstract_v<T>, "Cannot add abstract entity");
        static_assert(std::is_base_of_v<Entity, T>, "T must derive from Entity class");

        uint64_t id = m_entities.size();

        auto entity = std::make_unique<T>(entityName, std::forward<Args>(args)...);

        m_entities[id] = std::move(entity);

        return id;
    }

    Entity* getEntity(uint64_t id) const;

    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath);

    void update(float deltaTime);
    void draw(sf::RenderWindow& target);

private:
    //*Id = entity
    std::unordered_map<uint64_t, Entity::UniquePtr> m_entities;
};


#endif //SCENE_HPP