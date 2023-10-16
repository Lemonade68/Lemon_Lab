#include"Mirror.h"
#include"BxDF/Specular.h"

std::shared_ptr<BSDF> Mirror_Material::computeBSDF(const Intersection &intersection) const{
    //暂时不考虑法线贴图变化等
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    return std::make_shared<Specular_Reflection>(normal, tangent, bitangent);
}