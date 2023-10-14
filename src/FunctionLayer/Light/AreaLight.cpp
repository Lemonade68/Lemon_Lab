#include"AreaLight.h"

AreaLight::AreaLight(const std::shared_ptr<Shape> &ptr, const Spectrum &_energy) : Light(){
    type = LightType::AreaLight;
    energy = _energy;
    shape = ptr;
}

//面光源直接返回光强即可
Spectrum AreaLight::evaluateEmission(const Intersection &intersection, const Vector3f &wo) const{
    return energy;      
}

LightSampleResult AreaLight::sample(const Intersection &shadingPoint, const Vector2f &sample) const{
    Intersection sampleResult;
    float pdf;

    shape->uniformSampleOnSurface(sample, sampleResult, pdf);

    //方向是从shading point指向光源采样点
    Vector3f shadingPoint2sample = sampleResult.position - shadingPoint.position;

    return LightSampleResult{energy,
                             normalize(shadingPoint2sample),
                             shadingPoint2sample.length() - EPSILON,    //减EPSILON防止与自身相交
                             pdf,
                             type,
                             false,
                             sampleResult.normal};
}