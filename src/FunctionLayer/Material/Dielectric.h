#pragma once

#include"Material.h"
#include"NDF/Beckmann.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

class Dielectric_Material : public Material{
public:
    Dielectric_Material() = default;

    Dielectric_Material(const float _roughness, Vector3f _eta,
                       std::shared_ptr<NDF> _ndf = std::make_shared<Beckmann_ndf>(),
                       std::shared_ptr<Texture<Spectrum>> _albedo = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.8f)));

    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> albedo;      //��ɫֵ
    std::shared_ptr<NDF> ndf;       //���߷ֲ�����
    Vector3f eta;                   //���ϵ�������
    float roughness;                //���ϵĴֲڶȣ���Ӧbsdf�е�alpha��
};