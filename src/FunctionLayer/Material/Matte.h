#pragma once

#include"Material.h"

class Matte_Material : public Material{
public:
    Matte_Material(const Spectrum &_albedo = Spectrum(.5f)) : Material(), albedo(_albedo) {}

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;
    
private:
    //暂时不考虑材质(Texture)的问题，后面再更换成constant texture
    Spectrum albedo;
};