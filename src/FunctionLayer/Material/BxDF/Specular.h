#include"BSDF.h"

class Specular_Reflection : public BSDF{
public:
    Specular_Reflection(const Vector3f &_normal, const Vector3f &_tangent, const Vector3f &_bitangent)
        : BSDF(_normal, _tangent, _bitangent) {}
    
    //��ȫ���淴��ֱ�Ӳ�������Чֵ�ĸ���Ϊ0  ����  ֻ����һ����������Ϣ
    //sampleLight�������Ƕ�����0�ˣ�
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override{
        return Spectrum(.0f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f wiLocal(-woLocal[0], woLocal[1], -woLocal[2]);     //x z��ת��y����
        return {Spectrum(1.f), toWorld(wiLocal), 1.f, BSDFType::Specular};      //�������Ψһ�����pdfֱ��Ϊ1
    }

};