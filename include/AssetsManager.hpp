#ifndef ASSETS_MANAGER_HPP
#define ASSETS_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

//Fuck this is singletone. God, forgive me 
class AssetsManager
{
public:
    static AssetsManager& instance();

    sf::Texture* loadTexture(const std::string& filePath);

    sf::Texture* getTexture(const std::string& filePath);

    sf::Texture* addTexture(const sf::Texture& texture, const std::string& filePath);
    std::string getTexturePath(const sf::Texture* texture);

    const std::unordered_map<std::string, sf::Texture>& getTextures() const;

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
};

#endif //ASSETS_MANAGER_HPP