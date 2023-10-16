#pragma once
#include"Material.h"

class Mirror_Material : public Material{
public:
    Mirror_Material() : Material() {}

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;
};