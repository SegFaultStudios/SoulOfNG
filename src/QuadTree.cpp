#include "QuadTree.hpp"
#include <iostream>

QuadTree::QuadTree(const sf::FloatRect& bounds) : m_bounds(bounds) {}

std::vector<Entity*>QuadTree::findEntitiesAround(const sf::FloatRect& area) const {
    std::vector<Entity*> found;
    std::cout<<"searching around player: posX, posY: " << area.position.x << ", " << area.position.y << std::endl;
    if (!m_bounds.findIntersection(area)) {
        std::cout << "exited the note pos " << m_bounds.position.x << " " << m_bounds.position.y << std::endl;
        return found;
    }
    std::cout << "checking node with pos " << m_bounds.position.x << " " << m_bounds.position.y << std::endl;
    for (auto entity : m_entities) {

        if (entity->getGlobalBounds().findIntersection(area)) {
            found.push_back(entity);
        }
    }
    if (m_divided) {
        for (auto& child : m_children) {
            auto childFound = child->findEntitiesAround(area);
            found.insert(found.end(), childFound.begin(), childFound.end());
        }
    }
    return found;
}

bool QuadTree::contains(const sf::FloatRect& bounds) const {
    return m_bounds.contains({bounds.position.x, bounds.position.y}) &&
        m_bounds.contains({bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y});
}

bool QuadTree::insertInNode(Entity *entity) {

    auto bounds = entity->getGlobalBounds();
    if (!contains(bounds)) {
        return false;
    }

    if (!m_divided) {
        if (m_entities.size() < CAPACITY) {
            m_entities.push_back(entity);
            return true;
        }
        subdivide();
    }
    int index = getIndex(bounds);
    if (index == -1) {
        m_entities.push_back(entity);
        return true;
    }

    m_children[index]->insertInNode(entity);
    return true;

}

void QuadTree::subdivide() {
    float middleX = m_bounds.position.x + m_bounds.size.x / 2;
    float middleY = m_bounds.position.y + m_bounds.size.y / 2;
    float sizeX = m_bounds.size.x / 2;
    float sizeY = m_bounds.size.y / 2;

    sf::FloatRect firstArea({m_bounds.position.x, m_bounds.position.y}, {sizeX, sizeY});
    sf::FloatRect secondArea({middleX, m_bounds.position.y}, {sizeX, sizeY});
    sf::FloatRect thirdArea({m_bounds.position.x, middleY}, {sizeX, sizeY});
    sf::FloatRect fourthArea({middleX, middleY}, {sizeX, sizeY});

    m_children[0] = std::make_unique<QuadTree>(firstArea);
    m_children[1] = std::make_unique<QuadTree>(secondArea);
    m_children[2] = std::make_unique<QuadTree>(thirdArea);
    m_children[3] = std::make_unique<QuadTree>(fourthArea);

    m_divided = true;

    std::vector<Entity*> entitiesToMove;
    /*
    Any entity from the parent node that doesn't lie
    on the boundaries of the child nodes will be moved to the suitable child node.
    Entities on boundaries remain with the parent
    */

    for (auto& entity : m_entities) {
        int index = getIndex(entity->getGlobalBounds());
        if (index == -1) {
            entitiesToMove.push_back(entity);
        } else {
            m_children[index]->insertInNode(entity);
        }
    }
    m_entities = std::move(entitiesToMove);

}

int QuadTree::getIndex(const sf::FloatRect& bounds) const {

    float middlePosX = m_bounds.position.x + m_bounds.size.x / 2;
    float middlePosY = m_bounds.position.y + m_bounds.size.y / 2;

    bool fitsTop = (bounds.position.y + bounds.size.y) < middlePosY;
    bool fitsRight = bounds.position.x > middlePosX;
    bool fitsBottom = bounds.position.y > middlePosY;
    bool fitsLeft = (bounds.position.x + bounds.size.x) < middlePosX;

    if (fitsTop && fitsLeft) return 0;
    if (fitsTop && fitsRight) return 1;
    if (fitsBottom && fitsLeft) return 2;
    if (fitsBottom && fitsRight) return 3;
    return -1;
}

