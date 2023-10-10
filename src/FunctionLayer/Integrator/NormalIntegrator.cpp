#include"NormalIntegrator.h"

Spectrum NormalIntegrator::li(Ray &ray, const Scene &scene,
                        std::shared_ptr<Sampler> sampler) const{
    bool result = false, hit = false;
    auto intersection = scene.rayIntersect(ray);        //����std::optional�Ľ��
    if(!intersection.has_value())
        return Spectrum(.0f);
    return Spectrum((intersection.value().normal + Vector3f(1.f)) * .5f);   //������
}