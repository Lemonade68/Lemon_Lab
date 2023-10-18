#pragma once

#include"Integrator.h"

class WhittedIntegrator : public Integrator{
public:
    WhittedIntegrator(int _max_depth = 5) : max_depth(_max_depth) {}
    
    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;

private:
    int max_depth;      //最大弹射次数（镜面间）
};