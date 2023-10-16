#include"Matte.h"
#include"BxDF/Lambert.h"

std::shared_ptr<BSDF> Matte_Material::computeBSDF(const Intersection &intersection) const{
    //��ʱδ���Ƿ�����ͼ���ֵ�����
    Vector3f normal = intersection.normal,
             tangent = intersection.tangent,
             bitangent = intersection.bitangent;

    // ΪʲôMoer-lite��Ҫ���¼����µķ����ʣ�
    // �ǲ�����Ϊ��η��䵼�µ�ʵ���ϵķ�����Ҫָ���η����㣿
    return std::make_shared<Lambert_Reflection>(normal, tangent, bitangent, albedo);
}