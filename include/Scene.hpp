#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"
#include "Room.hpp"
#include "UI/UIWidget.hpp"

#include <memory>
#include <unordered_map>

#include "QuadTree.hpp"


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

        uint64_t id = ++m_nextEntityId;

        std::string finalName = entityName;

        if(doesEntityNameExist(entityName))
        {
            finalName = generateUniqueName(entityName);
        }

        auto entity = std::make_unique<T>(finalName, std::forward<Args>(args)...);

        m_entities[id] = std::move(entity);

        return id;
    }

    template<typename T>
    uint64_t addEntity(std::unique_ptr<T> entity)
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

        uint64_t id = ++m_nextEntityId;

        std::string finalName = entity->getName();

        if(doesEntityNameExist(entity->getName()))
        {
            finalName = generateUniqueName(entity->getName());
            entity->setName(finalName);
        }

        m_entities[id] = std::move(entity);

        return id;
    }



    //After this uiWidget will become invalid
    template<typename T>
    uint64_t addUI(std::unique_ptr<T> uiWidget)
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

        static_assert(!std::is_abstract_v<T>, "Cannot add abstract widget");
        static_assert(std::is_base_of_v<UIWidget, T>, "T must derive from Widget class");

        uint64_t id = ++m_nextUiWidgetId;

        std::string finalName = uiWidget->getName();

        if(doesUIWidgetNameExists(uiWidget->getName()))
        {
            finalName = generateUniqueName(uiWidget->getName());
            uiWidget->setName(finalName);
        }

        m_uiWidgets[id] = std::move(uiWidget);

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

        static_assert(!std::is_abstract_v<T>, "Cannot add abstract widget");
        static_assert(std::is_base_of_v<UIWidget, T>, "T must derive from Widget class");

        uint64_t id = ++m_nextUiWidgetId;

        std::string finalName = uiName;

        if(doesUIWidgetNameExists(uiName))
            finalName = generateUniqueName(uiName);

        auto uiWidget = std::make_unique<T>(finalName, std::forward<Args>(args)...);

        m_uiWidgets[id] = std::move(uiWidget);

        return id;
    }

    template<typename T>
    T* getEntity(uint64_t id) const
    {
        auto entity = getEntity(id);

        if(!entity)
            return nullptr;
        
        return dynamic_cast<T*>(entity);
    }

    Entity* getEntity(uint64_t id) const;

    template<typename T>
    T* getUiWidget(uint64_t id) const
    {
        auto widget = getUiWidget(id);

        if(!widget)
            return nullptr;

        return dynamic_cast<T*>(widget);
    }

    UIWidget* getUiWidget(uint64_t id) const;

    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath);
    const std::unordered_map<uint64_t, Entity::UniquePtr>& getEntities() const;
    const std::unordered_map<uint64_t, UIWidget::UniquePtr>& getUiWidgets() const;
    
    uint64_t findEntityWithName(const std::string& name) const;

    void handleInput(const sf::Event& event, const sf::RenderWindow& window);
    void update(float deltaTime);
    void draw(sf::RenderWindow& target);

    bool doesEntityNameExist(const std::string& name) const;
    bool doesUIWidgetNameExists(const std::string& name) const;

    bool removeEntity(uint64_t id);
    bool removeEntity(Entity* entity);

    void initQuadTree();
    sf::FloatRect getTwoEntitiesBounds(sf::FloatRect area1, sf::FloatRect area2) const;

    void setCurrentRoom(std::unique_ptr<Room> room);


private:
    //*Id = entity
    uint64_t m_nextEntityId = 0;
    uint64_t m_nextUiWidgetId = 0;
    std::unordered_map<uint64_t, Entity::UniquePtr> m_entities;
    std::unordered_map<uint64_t, UIWidget::UniquePtr> m_uiWidgets;
    std::unique_ptr<QuadTree> m_quadTree;



};


#endif //SCENE_HPP