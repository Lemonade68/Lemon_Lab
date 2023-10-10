#include"AreaLight.h"

AreaLight::AreaLight(const Spectrum &_energy, const std::shared_ptr<Shape> &ptr) : Light(){
    type = LightType::AreaLight;
    energy = _energy;
    shape = ptr;
}

//面光源直接返回光强即可
Spectrum AreaLight::evaluateEmission(const Intersection &intersection, const Vector3f &wo) const{
    return energy;      
}

//TODO：思考为什么要用LightSample？