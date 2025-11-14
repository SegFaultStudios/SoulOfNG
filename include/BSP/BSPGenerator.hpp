
#ifndef BSP_GENERATOR_HPP
#define BSP_GENERATOR_HPP
#include "BSPNode.hpp"
#include "Room.hpp"
#include <vector>
#include <random>


class BSPGenerator {
public:
    BSPGenerator(const sf::Vector2f& minSize, int depth, const sf::FloatRect& wholeArea);
    std::vector<sf::FloatRect> getTerritories();
private:
    std::unique_ptr<BSPNode> m_root;
    sf::FloatRect m_wholeArea;
    sf::Vector2f m_minTerritorySize;
    const int m_maxDepth;
    void splitNode(BSPNode* node, int currentDepth);
    void splitHorizontal(BSPNode* node, int depth);
    void splitVertical(BSPNode* node, int depth);
    void collectTerritoryRecursive(const BSPNode* node, std::vector<sf::FloatRect>& rooms);
    std::unique_ptr<BSPNode> generate();



};




#endif