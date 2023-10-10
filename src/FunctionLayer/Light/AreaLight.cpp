#include"AreaLight.h"

AreaLight::AreaLight(const Spectrum &_energy, const std::shared_ptr<Shape> &ptr) : Light(){
    type = LightType::AreaLight;
    energy = _energy;
    shape = ptr;
}

//���Դֱ�ӷ��ع�ǿ����
Spectrum AreaLight::evaluateEmission(const Intersection &intersection, const Vector3f &wo) const{
    return energy;      
}

//TODO��˼��ΪʲôҪ��LightSample��