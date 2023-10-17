#pragma once

#include"Material.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

class Matte_Material : public Material{
public:
    Matte_Material(const std::shared_ptr<Texture<Spectrum>> &_albedo = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f))){
        albedo = _albedo;
    }

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;
    
private:
    //暂时不考虑材质(Texture)的问题，后面再更换成constant texture
    // std::shared_ptr<ConstantTexture<Spectrum>> albedo;      //默认使用ConstantTexture
    std::shared_ptr<Texture<Spectrum>> albedo;      //默认使用ConstantTexture
};