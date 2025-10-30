#include "AssetsManager.hpp"

#include <iostream>

AssetsManager& AssetsManager::instance()
{
    static AssetsManager instance;
    return instance;
}

std::string AssetsManager::getTexturePath(const sf::Texture* texture)
{
    for(const auto& [path, tex] : m_textures)
        if(&tex == texture)
            return path;

    return {};
}

sf::Texture* AssetsManager::loadTexture(const std::string& filePath)
{
    if(auto it = m_textures.find(filePath); it != m_textures.end())
        return &it->second;

    sf::Texture texture;

    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture!" << std::endl;
        return nullptr;
    }

    texture.setSmooth(true);

    m_textures[filePath] = std::move(texture);

    return &m_textures[filePath];
}

void AssetsManager::addTexture(const sf::Texture& texture, const std::string& filePath)
{
    m_textures[filePath] = texture;
}

const std::unordered_map<std::string, sf::Texture>& AssetsManager::getTextures() const
{
    return m_textures;
}

sf::Texture* AssetsManager::getTexture(const std::string& filePath)
{
    auto it = m_textures.find(filePath);

    return it != m_textures.end() ? &it->second : nullptr;
}