#pragma once

#include"Material.h"

class Matte_Material : public Material{
public:
    Matte_Material(const Spectrum &_albedo = Spectrum(.5f)) : Material(), albedo(_albedo) {}

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;
    
private:
    //��ʱ�����ǲ���(Texture)�����⣬�����ٸ�����constant texture
    Spectrum albedo;
};