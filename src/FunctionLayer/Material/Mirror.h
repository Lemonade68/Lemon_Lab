#pragma once
#include"Material.h"

class Mirror_Material : public Material{
public:
    Mirror_Material(const Spectrum &_color = Spectrum(1.f)) : Material(), color(_color) {}

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;

private:
    Spectrum color;     //材质本身的颜色
};