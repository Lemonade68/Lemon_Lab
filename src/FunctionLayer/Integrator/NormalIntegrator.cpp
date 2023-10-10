#include"NormalIntegrator.h"

Spectrum NormalIntegrator::li(Ray &ray, const Scene &scene,
                        std::shared_ptr<Sampler> sampler) const{
    bool result = false, hit = false;
    auto intersection = scene.rayIntersect(ray);        //返回std::optional的结果
    if(!intersection.has_value())
        return Spectrum(.0f);
    return Spectrum((intersection.value().normal + Vector3f(1.f)) * .5f);   //处理法线
}