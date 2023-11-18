#include"Conductor.h"
#include"BxDF/RoughConductor.h"
#include"NDF/Beckmann.h"
#include"NDF/GGX.h"
#include"FunctionLayer/Texture/ConstantTexture.h"

Conductor_Material::Conductor_Material(const float _roughness, Vector3f _eta, Vector3f _eta_k,
                                       std::shared_ptr<NDF> _ndf, std::shared_ptr<Texture<Spectrum>> _albedo)
    : roughness(_roughness), eta(_eta), eta_k(_eta_k), ndf(_ndf), albedo(_albedo) { }


std::shared_ptr<BSDF> Conductor_Material::computeBSDF(const Intersection &intersection) const{
    //暂时未考虑法线贴图部分的问题
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    Spectrum s = albedo->evaluate(intersection);

    return std::make_shared<RoughConductorBSDF>(normal, tangent, bitangent, s, roughness, eta, eta_k, ndf);
}