#include"Matte.h"
#include"BxDF/Lambert.h"

std::shared_ptr<BSDF> Matte_Material::computeBSDF(const Intersection &intersection) const{
    //��ʱδ���Ƿ�����ͼ���ֵ�����
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    //��������õ㴦������ɫ��Ϣ���浽bsdf������
    Spectrum s = albedo->evaluate(intersection);

    return std::make_shared<Lambert_Reflection>(normal, tangent, bitangent, s);
}