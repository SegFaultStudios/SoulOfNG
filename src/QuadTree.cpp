#include "QuadTree.hpp"
#include <iostream>

QuadTree::QuadTree(const sf::FloatRect& bounds) : m_bounds(bounds) {}


std::vector<Entity*>QuadTree::findEntitiesAround(const sf::FloatRect& area) const {

    std::vector<Entity*> found;

    if (!m_bounds.findIntersection(area)) {
        return found;
    }
    /*std::cout << "======NEW NODE======" << std::endl;
    std::cout << "Current node pos: " << m_bounds.position.x << ", " << m_bounds.position.y << std::endl;
    std::cout << "Current node size: " << m_bounds.size.x << ", " << m_bounds.size.y << std::endl << std::endl;
    std::cout << "Searching area size: " << area.size.x << ", " << area.size.y << std::endl;
    std::cout << "Searching area position: " << area.position.x << ", " << area.position.y << std::endl;*/
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
    //std::cout << "=====NODE END=====" << std::endl << std::endl;

    return found;
}


bool QuadTree::contains(const sf::FloatRect& bounds) const {
    return m_bounds.contains({bounds.position.x, bounds.position.y}) &&
        m_bounds.contains({bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y});
}

bool QuadTree::insertInNode(Entity *entity) {

    auto bounds = entity->getGlobalBounds();
    if (!contains(bounds)) {
        //std::cout << "Entity is out of area, pos: " << bounds.position.x << ", " << bounds.position.y << std::endl
        //<< "size: " << bounds.size.x << ", " << bounds.size.y << std::endl << std::endl;
        //std::cout << "m_bounds position: " << m_bounds.position.x << ", " << m_bounds.position.y << std::endl <<
        //    "size: " << m_bounds.size.x << ", " << m_bounds.size.y << std::endl << std::endl;
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

int QuadTree::printAllEntities() const {
    int entityCount = 0;
    /*std::cout << "===NEW_NODE===" << std::endl;

    std::cout << "Node pos: " << m_bounds.position.x << ", " << m_bounds.position.y << std::endl;
    std::cout << "Node size: " << m_bounds.size.x << ", " << m_bounds.size.y << std::endl; */

    for (auto& entity : m_entities) {
        //std::cout << "entity pos: " << entity->getGlobalBounds().position.x << ", " << entity->getGlobalBounds().position.y << std::endl;
        //std::cout << "entity size: " << entity->getGlobalBounds().size.x << ", " << entity->getGlobalBounds().size.y << std::endl;
        entityCount++;
    }
    //std::cout << "===END_NODE===" << std::endl;

    if (m_divided)
        for (auto& child: m_children) {
            entityCount += child->printAllEntities();
        }
    return entityCount;

}

void QuadTree::drawSearchingNode(sf::RenderTarget& target) const {

    sf::RectangleShape leftBound;
    sf::RectangleShape rightBound;
    sf::RectangleShape topBound;
    sf::RectangleShape bottomBound;

    float thickness = 3.0f;

    leftBound.setPosition({
        m_bounds.position.x, m_bounds.position.y });

    rightBound.setPosition({
        m_bounds.position.x + m_bounds.size.x, m_bounds.position.y });

    topBound.setPosition({
        m_bounds.position.x, m_bounds.position.y });

    bottomBound.setPosition({
        m_bounds.position.x, m_bounds.position.y + m_bounds.size.y
        });

    leftBound.setSize({ thickness, m_bounds.size.y });
    rightBound.setSize({ thickness, m_bounds.size.y });
    topBound.setSize({ m_bounds.size.x, thickness });
    bottomBound.setSize({ m_bounds.size.x, thickness });


    leftBound.setFillColor(sf::Color::Green);
    rightBound.setFillColor(sf::Color::Green);
    topBound.setFillColor(sf::Color::Green);
    bottomBound.setFillColor(sf::Color::Green);

    
}

void QuadTree::drawNodes(sf::RenderTarget& target) const {

    sf::RectangleShape leftBound;
    sf::RectangleShape rightBound;
    sf::RectangleShape topBound;
    sf::RectangleShape bottomBound;

    float thickness = 3.0f;

    leftBound.setPosition({
        m_bounds.position.x, m_bounds.position.y });

    rightBound.setPosition({
        m_bounds.position.x + m_bounds.size.x, m_bounds.position.y });

    topBound.setPosition({
        m_bounds.position.x, m_bounds.position.y });

    bottomBound.setPosition({
        m_bounds.position.x, m_bounds.position.y + m_bounds.size.y
        });

    leftBound.setSize({ thickness, m_bounds.size.y });
    rightBound.setSize({ thickness, m_bounds.size.y });
    topBound.setSize({ m_bounds.size.x, thickness });
    bottomBound.setSize({ m_bounds.size.x, thickness });

    
    leftBound.setFillColor(sf::Color::Red);
    rightBound.setFillColor(sf::Color::Red);
    topBound.setFillColor(sf::Color::Red);
    bottomBound.setFillColor(sf::Color::Red);

    if (m_divided) {
        for (auto& child : m_children) {
            child->drawNodes(target);
        }
    }
    target.draw(leftBound);
    target.draw(rightBound);
    target.draw(topBound);
    target.draw(bottomBound);
}


