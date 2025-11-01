#ifndef RAY_CASTING_HPP
#define RAY_CASTING_HPP

#include "Scene.hpp"

#include "Ray.hpp"

struct RaycastResult
{
    void* hit;
    float distance;
};

class Raycasting
{
public:
    static void init(Scene* scene);

    bool castRay(const Ray& ray, RaycastResult& result);

private:
    static inline Scene* s_scene{nullptr};
};


#endif //RAY_CASTING_HPP