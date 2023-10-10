#pragma once

#include"Integrator.h"

//法线可视化（测试使用）
class NormalIntegrator : public Integrator{
public:
    NormalIntegrator() = default;
    virtual ~NormalIntegrator() = default;

    virtual Spectrum li(Ray &ray, const std::vector<std::shared_ptr<Shape>> &scene,
                        std::shared_ptr<Sampler> sampler) const override;
};