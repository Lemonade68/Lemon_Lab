#pragma once

#include "Integrator.h"

class PathIntegrator : public Integrator{
public:
    PathIntegrator(const int &_max_depth = 5) : max_depth(_max_depth) {}

    virtual Spectrum li(Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler) const override;

private:
    int max_depth;
};