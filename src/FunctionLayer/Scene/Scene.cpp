#include"Scene.h"

std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    bool hit = false, result = false;
    Intersection intersection;
    for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
        result = (*it)->rayIntersectShape(ray, intersection);       //场景中物体与光线求交
        if(result)
            hit = result;
    }
    if(!hit)
        return std::nullopt;

    return std::make_optional(intersection);
}