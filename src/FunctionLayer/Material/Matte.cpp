#include"Matte.h"
#include"BxDF/Lambert.h"

std::shared_ptr<BSDF> Matte_Material::computeBSDF(const Intersection &intersection) const{
    //暂时未考虑法线贴图部分的问题
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    //计算物体该点处材质颜色信息，存到bsdf材质中
    Spectrum s = albedo->evaluate(intersection);

    return std::make_shared<Lambert_Reflection>(normal, tangent, bitangent, s);
}