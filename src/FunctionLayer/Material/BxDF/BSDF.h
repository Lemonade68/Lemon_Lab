#pragma once
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Shape/Intersection.h"

enum class BSDFType {
    Diffuse,
    Specular
};


class BSDF{
public:
    BSDF(const Vector3f &_normal, const Vector3f &_tangent,
         const Vector3f &_bitangent){
        normal = _normal;
        tangent = _tangent;
        bitangent = _bitangent;
    }

    //����wi��wo��Ӧ��BSDFֵ(��ʽ�е�F��)
    virtual Spectrum F(const Vector3f &wo, const Vector3f &wi) const = 0;

    //TODO����BSDF�ϲ����ķ���
    

public:
    //���ɸõ��µľֲ�����ϵ(��������������ϵ�µ�ֵ)(Ӧ���ǵ�λ������)
    Vector3f normal, tangent, bitangent;

protected:
    //����ռ�������ɾֲ��ռ䣺ֱ��ͶӰ�������������ϼ���
    Vector3f toLocal(const Vector3f &world) const{
        return Vector3f{dot(world, tangent), dot(world, normal), dot(world, bitangent)};
    }

    //�ֲ��ռ������������ռ䣺���ϻ�����������ռ�ı�ʾ
    Vector3f toWorld(const Vector3f &local) const{
        return local.x() * tangent + local.y() * normal + local.z() * bitangent;
    }
};