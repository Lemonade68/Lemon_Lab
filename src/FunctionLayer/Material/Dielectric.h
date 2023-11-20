#pragma once

#include"Material.h"
#include"NDF/Beckmann.h"
#include"NDF/GGX.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

class Dielectric_Material : public Material{
public:
    Dielectric_Material() = default;

    Dielectric_Material(const float _roughness, Vector3f _eta,
                       std::shared_ptr<NDF> _ndf = std::make_shared<GGX>(),
                       std::shared_ptr<Texture<Spectrum>> _albedo = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.8f)));

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> albedo;      //颜色值
    std::shared_ptr<NDF> ndf;       //法线分布函数
    Vector3f eta;                   //相对折射率，透射侧/入射侧
    Vector3f inv_eta;               //相对折射率的倒数，入射侧/透射侧
    float roughness;                //材料的粗糙度（对应bsdf中的alpha）
};