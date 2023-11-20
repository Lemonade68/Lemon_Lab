#include"Dielectric.h"
#include"BxDF/RoughDielectric.h"
#include"NDF/Beckmann.h"
#include"NDF/GGX.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

Dielectric_Material::Dielectric_Material(const float _roughness, Vector3f _eta,
                                         std::shared_ptr<NDF> _ndf, std::shared_ptr<Texture<Spectrum>> _albedo)
    : roughness(_roughness), eta(_eta), ndf(_ndf), albedo(_albedo) { inv_eta = Vector3f(1.f / eta[0], 1.f / eta[1], 1.f / eta[2]); }

std::shared_ptr<BSDF> Dielectric_Material::computeBSDF(const Intersection &intersection) const{
    //暂时未考虑法线贴图部分的问题
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    Spectrum s = albedo->evaluate(intersection);

    return std::make_shared<RoughDielectricBSDF>(normal, tangent, bitangent, s, roughness, eta, ndf);
}