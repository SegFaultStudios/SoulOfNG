#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"
#include "UI/UIWidget.hpp"

#include <memory>
#include <unordered_map>

class Scene
{
public:
    template<typename T, typename... Args>
    uint64_t addEntity(const std::string& entityName, Args&&... args)
    {
        auto generateUniqueName = [this](const std::string& baseName)
        {
            int counter = 1;
            std::string newName;
            
            do 
            {
                newName = baseName + "_" + (counter < 10 ? "0" : "") + std::to_string(counter);
                counter++;
            }
            while (doesEntityNameExist(newName));
            
            return newName;
        };

        static_assert(!std::is_abstract_v<T>, "Cannot add abstract entity");
        static_assert(std::is_base_of_v<Entity, T>, "T must derive from Entity class");

        uint64_t id = m_entities.size();

        std::string finalName = entityName;

        if(doesEntityNameExist(entityName))
        {
            finalName = generateUniqueName(entityName);
        }

        auto entity = std::make_unique<T>(finalName, std::forward<Args>(args)...);

        m_entities[id] = std::move(entity);

        return id;
    }
    template<typename T, typename... Args>
    uint64_t addUI(const std::string& uiName, Args&&... args)
    {
        auto generateUniqueName = [this](const std::string& baseName)
        {
            int counter = 1;
            std::string newName;
            
            do 
            {
                newName = baseName + "_" + (counter < 10 ? "0" : "") + std::to_string(counter);
                counter++;
            }
            while (doesUIWidgetNameExists(newName));
            
            return newName;
        };

        static_assert(!std::is_abstract_v<T>, "Cannot add abstract entity");
        static_assert(std::is_base_of_v<UIWidget, T>, "T must derive from Entity class");

        uint64_t id = m_uiWidgets.size();

        std::string finalName = uiName;

        if(doesUIWidgetNameExists(uiName))
            finalName = generateUniqueName(uiName);

        auto uiWidget = std::make_unique<T>(finalName, std::forward<Args>(args)...);

        m_uiWidgets[id] = std::move(uiWidget);

        return id;
    }


    Entity* getEntity(uint64_t id) const;

    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath);
    const std::unordered_map<uint64_t, Entity::UniquePtr>& getEntities() const;
    const std::unordered_map<uint64_t, UIWidget::UniquePtr>& getUiWidgets() const;
    
    uint64_t findEntityWithName(const std::string& name) const;

    void handleInput(sf::Event& event);
    void update(float deltaTime);
    void draw(sf::RenderWindow& target);

    bool doesEntityNameExist(const std::string& name) const;
    bool doesUIWidgetNameExists(const std::string& name) const;

    bool removeEntity(uint64_t id);
    bool removeEntity(Entity* entity);

private:
    //*Id = entity
    std::unordered_map<uint64_t, Entity::UniquePtr> m_entities;
    std::unordered_map<uint64_t, UIWidget::UniquePtr> m_uiWidgets;
};


#endif //SCENE_HPP