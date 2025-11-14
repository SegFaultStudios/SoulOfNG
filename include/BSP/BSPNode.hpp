
#ifndef BSP_NODE_HPP
#define BSP_NODE_HPP
#include <SFML/Graphics.hpp>

class BSPNode {

public:
    BSPNode(const sf::FloatRect &rect) : m_area(rect) {}


private:
    sf::FloatRect m_area;
    std::unique_ptr<BSPNode> m_childA;
    std::unique_ptr<BSPNode> m_childB;
    bool m_isLeaf = false;

    friend class BSPGenerator;



};
#endif