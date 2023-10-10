#pragma once

#include"BSDF.h"

class Lambert_Reflection : public BSDF{
public:
    Lambert_Reflection(const Vector3f &_normal, const Vector3f &_tangent,
            const Vector3f &_bitangent, const Spectrum &_albedo)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo) {}

    //Lambert���ʶ�Ӧ��BSDFֵ������������ͬ��Ϊ����*cos�ȣ�
    virtual Spectrum F(const Vector3f &wo, const Vector3f &wi) const override{
        Vector3f wiLocal = normalize(toLocal(wi));          //��ɾֲ��ռ�
        return albedo * INV_PI * wiLocal[1];                //wiLocal[1]��ֵ����cos�ȵ�ֵ
    }

    //TODO��BSDF�ϲ���
    
private:
    Spectrum albedo;    //������
};
