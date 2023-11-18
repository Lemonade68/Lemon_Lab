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
    std::shared_ptr<Texture<Spectrum>> albedo;      //��ɫֵ
    std::shared_ptr<NDF> ndf;       //���߷ֲ�����
    Vector3f eta;                   //���ϵ�������
    Vector3f eta_k;                 //���������ʵ��鲿������ϵ����
    float roughness;                //���ϵĴֲڶȣ���Ӧbsdf�е�alpha��
};