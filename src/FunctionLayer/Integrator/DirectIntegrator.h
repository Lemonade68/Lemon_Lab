#pragma once

#include"Integrator.h"

//直接采样shading point
class DirectIntegrator_SampleBSDF : public Integrator{
public:
    DirectIntegrator_SampleBSDF() = default;

    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;
};


//采样光源，注意需要将pdf进行修改
class DirectIntegrator_SampleLight : public Integrator{
public:
    DirectIntegrator_SampleLight() = default;

    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;
};
