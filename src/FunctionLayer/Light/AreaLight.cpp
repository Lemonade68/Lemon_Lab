#include"AreaLight.h"

AreaLight::AreaLight(const std::shared_ptr<Shape> &ptr, const Spectrum &_energy) : Light(){
    type = LightType::AreaLight;
    energy = _energy;
    shape = ptr;
}

//���Դֱ�ӷ��ع�ǿ����
Spectrum AreaLight::evaluateEmission(const Intersection &intersection, const Vector3f &wo) const{
    return energy;      
}

LightSampleResult AreaLight::sample(const Intersection &shadingPoint, const Vector2f &sample) const{
    Intersection sampleResult;
    float pdf;

    shape->uniformSampleOnSurface(sample, sampleResult, pdf);

    //�����Ǵ�shading pointָ���Դ������
    Vector3f shadingPoint2sample = sampleResult.position - shadingPoint.position;

    return LightSampleResult{energy,
                             normalize(shadingPoint2sample),
                             shadingPoint2sample.length() - EPSILON,    //��EPSILON��ֹ�������ཻ
                             pdf,
                             type,
                             false,
                             sampleResult.normal};
}