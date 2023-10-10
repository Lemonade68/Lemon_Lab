#include"Matte.h"
#include"BxDF/Lambert.h"

std::shared_ptr<BSDF> Matte_Material::computeBSDF(const Intersection &intersection) const{
    //暂时未考虑法线贴图部分的问题
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    //为什么Moer-lite里要重新计算新的反射率？
    return std::make_shared<Lambert_Reflection>(normal, tangent, bitangent, albedo);
}