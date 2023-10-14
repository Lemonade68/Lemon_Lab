#pragma once

#include"Integrator.h"

//ֱ�Ӳ���shading point
class DirectIntegrator_SampleBSDF : public Integrator{
public:
    DirectIntegrator_SampleBSDF() = default;

    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;
};


//������Դ��ע����Ҫ��pdf�����޸�
class DirectIntegrator_SampleLight : public Integrator{
public:
    DirectIntegrator_SampleLight() = default;

    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;
};
