#pragma once

#include"Material.h"
#include"NDF/Beckmann.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

class Conductor_Material : public Material{
public:
    Conductor_Material() = default;

    Conductor_Material(const float _roughness, Vector3f _eta, Vector3f _eta_k,
                       std::shared_ptr<NDF> _ndf = std::make_shared<Beckmann_ndf>(),
                       std::shared_ptr<Texture<Spectrum>> _albedo = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.8f)));

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> albedo;      //颜色值
    std::shared_ptr<NDF> ndf;       //法线分布函数
    Vector3f eta;                   //材料的折射率
    Vector3f eta_k;                 //导体折射率的虚部（消光系数）
    float roughness;                //材料的粗糙度（对应bsdf中的alpha）
};