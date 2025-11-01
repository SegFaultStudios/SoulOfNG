#include "Physics/Raycasting.hpp"
#include "Utilities.hpp"

void Raycasting::init(Scene* scene)
{
    s_scene = scene;
}

bool Raycasting::castRay(const Ray& ray, RaycastResult& result)
{
    // const auto& entities = s_scene->getEntities();

    // sf::Vector2f normalizedDirection = utilities::normalize(ray.direction);

    // for(const auto& [id, entity] : entities)
    // {

    // }

    return false;
}
