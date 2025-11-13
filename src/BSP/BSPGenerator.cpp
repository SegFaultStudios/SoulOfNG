#include "BSP/BSPGenerator.hpp"
#include <random>

BSPGenerator::BSPGenerator (const sf::Vector2f& minSize, int depth, const sf::FloatRect& wholeArea)
    : m_minTerritorySize(minSize), m_maxDepth(depth), m_wholeArea(wholeArea) {
    m_root = generate();
}

std::unique_ptr<BSPNode> BSPGenerator::generate() {
    auto root = std::make_unique<BSPNode>(m_wholeArea);
    splitNode(root.get(), 0);
    return root;
}

std::vector<sf::FloatRect> BSPGenerator::getTerritories() {
    std::vector<sf::FloatRect> territories;
    collectTerritoryRecursive(m_root.get(), territories);
    return territories;
}

void BSPGenerator::splitNode(BSPNode* node, int currentDepth) {

    if (currentDepth >= m_maxDepth) {
        node->m_isLeaf = true;
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    bool isSplitHorizontal = distrib(gen);
    if (isSplitHorizontal) {
        if ((node->m_area.size.y) / 2 < m_minTerritorySize.y ) {
            node->m_isLeaf = true;
            return;
        }
        splitHorizontal(node, currentDepth);


    } else {
        if ((node->m_area.size.x) / 2 < m_minTerritorySize.x ) {
            node->m_isLeaf = true;
            return;
        }
        splitVertical(node, currentDepth);

    }
}

void BSPGenerator::splitHorizontal(BSPNode* node, int currentDepth) {
    float splitYMinPos = node->m_area.position.y + m_minTerritorySize.y;
    float splitYMaxPos = node->m_area.position.y + node->m_area.size.y - m_minTerritorySize.y;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(splitYMinPos, splitYMaxPos);
    float splitYPos = distrib(gen);

    float childASizeY = splitYPos - node->m_area.position.y;
    sf::FloatRect areaChildA(
        sf::Vector2f(node->m_area.position.x, node->m_area.position.y),
        sf::Vector2f(node->m_area.size.x, childASizeY));

    float childBSizeY = node->m_area.size.y - childASizeY;
    sf::FloatRect areaChildB(
        sf::Vector2f(node->m_area.position.x, splitYPos),
        sf::Vector2f(node->m_area.size.x, childBSizeY));

    node->m_childA = std::make_unique<BSPNode>(areaChildA);
    node->m_childB = std::make_unique<BSPNode>(areaChildB);

    splitNode(node->m_childA.get(), currentDepth + 1);
    splitNode(node->m_childB.get(), currentDepth + 1);

}

void BSPGenerator::splitVertical(BSPNode* node, int currentDepth) {
    float splitXMinPos = node->m_area.position.x + m_minTerritorySize.x;
    float splitXMaxPos = node->m_area.position.x + node->m_area.size.x - m_minTerritorySize.x;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(splitXMinPos, splitXMaxPos);
    float splitXPos = distrib(gen);

    float childASizeX = splitXPos - node->m_area.position.x;
    sf::FloatRect areaChildA(
        sf::Vector2f(node->m_area.position.x, node->m_area.position.y),
        sf::Vector2f(childASizeX, node->m_area.size.y ));

    float childBSizeX = node->m_area.size.x - childASizeX;
    sf::FloatRect areaChildB(
        sf::Vector2f(splitXPos, node->m_area.position.y),
        sf::Vector2f(childBSizeX, node->m_area.size.y));

    node->m_childA = std::make_unique<BSPNode>(areaChildA);
    node->m_childB = std::make_unique<BSPNode>(areaChildB);

    splitNode(node->m_childA.get(), currentDepth + 1);
    splitNode(node->m_childB.get(), currentDepth + 1);

}

void BSPGenerator::collectTerritoryRecursive(const BSPNode *node, std::vector<sf::FloatRect> &rooms) {
    if (!node) return;

    if (node->m_isLeaf) {
        rooms.push_back(node->m_area);
    } else {
        collectTerritoryRecursive(node->m_childA.get(), rooms);
        collectTerritoryRecursive(node->m_childB.get(), rooms);
    }
}
