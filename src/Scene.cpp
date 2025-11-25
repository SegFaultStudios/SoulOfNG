#include "Scene.hpp"
#include "CollisionHandler.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <random>

#include "Entities/Player.hpp"
#include "Entities/Wall.hpp"

Entity* Scene::getEntity(uint64_t id) const
{
    auto it = m_entities.find(id);

    return it == m_entities.end() ? nullptr : it->second.get();
}

UIWidget* Scene::getUiWidget(uint64_t id) const
{
    auto it = m_uiWidgets.find(id);

    return it == m_uiWidgets.end() ? nullptr : it->second.get();
}

bool Scene::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " <<  filePath << std::endl;
        return false;
    }

    nlohmann::json json;

    try
    {
        file >> json;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "Failed to parse scene file " << e.what() << std::endl;
        return false;
    }

    if (!json.contains("game_objects"))
    {
        std::cerr << "No game objects" << std::endl;
        return false;
    }

    for (const auto& objectJson : json["game_objects"])
    {
        const std::string& name = objectJson.value("name", "undefined");
        const std::string& texturePath = objectJson.value("texture", "");

        if(!objectJson.contains("type"))
        {
            std::cerr << "No type " << std::endl;
            continue;
        }

        int typeId = objectJson["type"];

        EntityEnum type = static_cast<EntityEnum>(typeId);

        Entity* entity{nullptr};

        if(type == EntityEnum::PLAYER)
        {
            auto id = addEntity<Player>(name);
            entity = getEntity(id);
        }
        else if(type == EntityEnum::WALL)
        {
            auto id = addEntity<Wall>(name);
            entity = getEntity(id);
        }
        else
        {   
            auto id = addEntity<Entity>(name);
            entity = getEntity(id);
        }

        if(!entity)
        {
            std::cerr << "Failed to parse entity" << std::endl;
            continue;
        }

        if(!texturePath.empty())
        {
            if(auto texture = AssetsManager::instance().loadTexture(texturePath))
                entity->setTexture(*texture);
            else
                std::cerr << "Failed to set texture: " << texturePath << '\n';
        }

        if (objectJson.contains("position"))
        {
            const auto& pos = objectJson["position"];
            entity->setPosition({pos[0], pos[1]});
        }

        if (objectJson.contains("scale"))
        {
            const auto& scale = objectJson["scale"];
            entity->setScale({ scale[0], scale[1]});
        }

        // if (objectJson.contains("rotation"))
        // {
            // const auto& rot = objectJson["rotation"];
            // entity->setRotation({rot[0], rot[1]});
        // }
    }

    file.close();

    return true;
}

bool Scene::saveToFile(const std::string& filePath)
{
    nlohmann::json json;

    //TODO Scene name
    // json["name"] = m_name.empty() ?  std::filesystem::path(filePath).filename().string() : m_name;

    for (const auto& [id, entity] : m_entities)
    {
        nlohmann::json objectJson;

        objectJson["name"] = entity->getName();
        objectJson["texture"] = AssetsManager::instance().getTexturePath(&entity->getTexture());
        objectJson["position"] = {entity->getPosition().x, entity->getPosition().y};
        objectJson["scale"] = {entity->getScale().x, entity->getScale().y};

        //TODO this is soo bad...
        if(auto player = dynamic_cast<Player*>(entity.get()))
            objectJson["type"] = 1;
        else if(auto wall = dynamic_cast<Wall*>(entity.get()))
            objectJson["type"] = 2;
        else
            objectJson["type"] = 0;

        // objectJson["rotation"] = {entity->getRotation().x, entity->getRotation().y};

        json["game_objects"].push_back(objectJson);
    }

    std::ofstream file(filePath);

    if (file.is_open())
    {
        file << std::setw(4) << json << std::endl;
        file.close();
    }
    else
        std::cerr << "Failed to open file to save game objects: " << filePath << std::endl;

    return true;
}

bool Scene::doesEntityNameExist(const std::string& name) const
{
    for(const auto& [id, entity] : m_entities)
        if(entity->getName() == name)
            return true;

    return false;
}

bool Scene::doesUIWidgetNameExists(const std::string& name) const
{
    for(const auto& [id, uiWidget] : m_uiWidgets)
        if(uiWidget->getName() == name)
            return true;

    return false;
}

void Scene::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
    if (auto* resized = event.getIf<sf::Event::Resized>()) 
    {
        sf::Vector2f newSize(resized->size.x, resized->size.y);
        m_uiView.setSize(newSize);
    }

    for(const auto& [id, entity] : m_entities)
        entity->handleInput(event);

    
    auto currentView = window.getView();

    window.setView(m_uiView);

    for(const auto& [id, uiWidget] : m_uiWidgets)
        uiWidget->handleEvent(event, window);

    window.setView(currentView);
}

void Scene::initQuadTree() {
    if (m_entities.empty()) return;
    sf::FloatRect worldBounds;
     
    for (const auto& [id, entity] : m_entities) {
        if (!dynamic_cast<Player*>(entity.get()))
        {
            auto bounds = entity->getGlobalBounds();
            worldBounds = getTwoEntitiesBounds(worldBounds, bounds);
            
        }
    }
    float epsilon = 1.0f;
    worldBounds = sf::FloatRect({worldBounds.position.x - epsilon, worldBounds.position.y - epsilon},
        {worldBounds.size.x + 2 * epsilon, worldBounds.size.y + 2 * epsilon});

    std::cout<< "the parrent's size: " << worldBounds.size.x << ", " << worldBounds.size.y << std::endl << std::endl;
    m_quadTree = std::make_unique<QuadTree>(worldBounds);


    for (const auto& [id, entity] : m_entities) {
        if (!dynamic_cast<Player*>(entity.get()))
        {
            m_quadTree->insertInNode(entity.get());
        }
    }
    std::cout << m_quadTree->printAllEntities();

 }

sf::FloatRect Scene::getTwoEntitiesBounds(sf::FloatRect area1, sf::FloatRect area2) const {

    float left = std::min(area1.position.x, area2.position.x);
    float right = std::max(area1.position.x + area1.size.x, area2.position.x + area2.size.x);
    float bottom = std::max(area1.position.y + area1.size.y, area2.position.y + area2.size.y);
    float top = std::min(area1.position.y, area2.position.y);

    return sf::FloatRect({left, top}, {right - left, bottom - top} );
}

void Scene::update(float deltaTime)
{
   // Player* player = nullptr;
    /*sf::FloatRect playerBounds;

    for (auto& [id, entity] : m_entities) {
        if (Player* p = dynamic_cast<Player*>(entity.get())) {
            player = p;
            playerBounds = player->getGlobalBounds();
            break;
        }
    }*/

    for (const auto& [id, entity] : m_entities)
        entity->update(deltaTime);

    for (const auto& [id, uiWidget] : m_uiWidgets)
        uiWidget->update(deltaTime);




    //if (player) {

        //auto potentialPlayerPosition = player->getPotentialPosition();
        //
        //    float searchRadius = 80.0f;

        //    sf::FloatRect area(
        //        { playerBounds.position.x - searchRadius,
        //        playerBounds.position.y - searchRadius },
        //        { searchRadius * 2, searchRadius * 2 });

        //    auto entitiesAround = m_quadTree->findEntitiesAround(area);
        //    //std::cout << "Entities around: " << entitiesAround.size() << std::endl;
        //    bool pidoras = false;
        //    for (auto& entity : entitiesAround) {
        //        auto entityBounds = entity->getGlobalBounds();

        //        float epsilon = 0.01f;

        //        if (playerBounds.findIntersection(entityBounds)) {
        //            pidoras = true;
        //            //std::cout << "gay found" << std::endl;
        //            auto pos = CollisionHandler::resolveNoRotationWallMovement(player->getGlobalBounds(), potentialPlayerPosition, entityBounds);
        //            std::cout << "pos x: " << pos.x << " y: " << pos.y << std::endl;
        //            player->setPosition(pos);

        //        }

        //    }
        //    if (!pidoras) {
        //        player->setPosition(player->getPosition() + potentialPlayerPosition);
        //    }
        

        //}
    }




const std::unordered_map<uint64_t, Entity::UniquePtr>& Scene::getEntities() const
{
    return m_entities;
}

const std::unordered_map<uint64_t, UIWidget::UniquePtr>& Scene::getUiWidgets() const
{
    return m_uiWidgets;
}

void Scene::draw(sf::RenderWindow& target)
{
    for(const auto& [id, entity] : m_entities)
        entity->draw(target);

    //UI elements should always be on the screen, so use default view here

    auto currentView = target.getView();
    target.setView(m_uiView);

    for(const auto& [id, uiWidget] : m_uiWidgets)
        target.draw(*uiWidget);

    target.setView(currentView);
    if (m_quadTree)
        m_quadTree->drawNodes(target);
    
}

uint64_t Scene::findEntityWithName(const std::string& name) const
{
    for(const auto& [id, entity] : m_entities)
        if(entity->getName() == name)
            return id;

    return 0;
}

bool Scene::removeEntity(uint64_t id)
{
    auto it = m_entities.find(id);

    if(it != m_entities.end())
    {
        m_entities.erase(it);
        return true;
    }

    return false;
}

bool Scene::removeEntity(Entity* entity)
{
    for(const auto& [id, en] : m_entities)
        if(en.get() == entity)
        {
            m_entities.erase(id);
            return true;
        }

    return false;
}