#pragma once

#include"Integrator.h"

//���߿��ӻ�������ʹ�ã�
class NormalIntegrator : public Integrator{
public:
    NormalIntegrator() = default;
    virtual ~NormalIntegrator() = default;

    virtual Spectrum li(Ray &ray, const std::vector<std::shared_ptr<Shape>> &scene,
                        std::shared_ptr<Sampler> sampler) const override;
};