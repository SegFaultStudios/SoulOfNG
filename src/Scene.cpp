#include "Scene.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <random>

#include "Player.hpp"
#include "Wall.hpp"

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

void Scene::handleInput(const sf::Event& event, const sf::RenderWindow& window)
{
    for(const auto& [id, entity] : m_entities)
        entity->handleInput(event);

    for(const auto& [id, uiWidget] : m_uiWidgets)
        uiWidget->handleEvent(event, window);
}

void Scene::update(float deltaTime)
{
    Player* player = nullptr;
    sf::Vector2f oldPlayerPos;

    for (auto& [id, entity] : m_entities) {
        if (Player *p = dynamic_cast<Player*>(entity.get())) {
            player = p;
            oldPlayerPos = player->getPosition();
            break;
        }
    }

    for(const auto& [id, entity] : m_entities)
        entity->update(deltaTime);

    for(const auto& [id, uiWidget] : m_uiWidgets)
        uiWidget->update(deltaTime);

    for (auto& [id, entity] : m_entities) {
        if (dynamic_cast<Wall*>(entity.get())) {
            if (player->getGlobalBounds().findIntersection(entity->getGlobalBounds())) {
                player->setPosition(oldPlayerPos);
                break;
            }
        }
    }
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
    target.setView(target.getDefaultView());

    for(const auto& [id, uiWidget] : m_uiWidgets)
        target.draw(*uiWidget);

    target.setView(currentView);
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