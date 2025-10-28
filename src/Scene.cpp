#include "Scene.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

#include "Player.hpp"

Entity* Scene::getEntity(uint64_t id) const
{
    auto it = m_entities.find(id);

    return it == m_entities.end() ? nullptr : it->second.get();
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
            auto id = addEntity<Player>("enity");
            entity = getEntity(id);
        }
        else
        {   
            std::cerr << "Something is weird with id" << std::endl;
            continue;
        }

        if(!entity)
        {
            std::cerr << "Failed to parse entity" << std::endl;
            continue;
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

        objectJson["position"] = {entity->getPosition().x, entity->getPosition().y};
        objectJson["scale"] = {entity->getScale().x, entity->getScale().y};

        //TODO this is soo bad...
        if(auto player = dynamic_cast<Player*>(entity.get()))
        {
            objectJson["type"] = 1;
        }

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

void Scene::handleInput(sf::Event& event)
{
    for(const auto& [id, entity] : m_entities)
        entity->handleInput(event);
}

void Scene::update(float deltaTime)
{
    for(const auto& [id, entity] : m_entities)
        entity->update(deltaTime);
}

void Scene::draw(sf::RenderWindow& target)
{
    for(const auto& [id, entity] : m_entities)
        entity->draw(target);
}
