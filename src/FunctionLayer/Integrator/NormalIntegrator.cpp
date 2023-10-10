#include"NormalIntegrator.h"

Spectrum NormalIntegrator::li(Ray &ray, const std::vector<std::shared_ptr<Shape>> &scene,
                        std::shared_ptr<Sampler> sampler) const{
    bool result = false, hit = false;
    Intersection intersection;
    //计算scene中的物体与光线是否有交点
    for (auto it = scene.begin(); it != scene.end(); ++it){
        result = (*it)->rayIntersectShape(ray, intersection);
        if(result)
            hit = result;
    }

    if(!hit)
        return Spectrum(.0f);

    return Spectrum((intersection.normal + Vector3f(1.f)) * .5f);   //处理法线
}