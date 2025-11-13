
#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP

#include "Entity.hpp"
#include <SFML/Graphics.hpp>


class QuadTree {

public:
    QuadTree(const sf::FloatRect& bounds);
    std::vector<Entity*>findEntitiesAround(const sf::FloatRect& area) const;
    void clear();
    void draw(sf::RenderWindow& window) const;


private:
    static const int CAPACITY = 4;
    sf::FloatRect m_bounds;
    std::vector<Entity*> m_entities;
    std::array<std::unique_ptr<QuadTree>, 4> m_children;
    bool m_divided = false;
    void subdivide();
    int getIndex(const sf::FloatRect& bounds) const;
    bool contains(const sf::FloatRect& bounds) const;
    bool insertInNode(Entity* entity);

};




#endif
