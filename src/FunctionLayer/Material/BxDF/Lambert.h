#pragma once

//Lambert������ģ����Oren-Nayar������ģ������https://zhuanlan.zhihu.com/p/500809166

#include"BSDF.h"
#include"Sampling.h"

class Lambert_Reflection : public BSDF{
public:
    Lambert_Reflection(const Vector3f &_normal, const Vector3f &_tangent,
            const Vector3f &_bitangent, const Spectrum &_albedo)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo) {}

    //΢����ɢ��ģ���У�Lambert���ʶ�Ӧ��BSDFֵ������������ͬ��Ϊ����*cos�ȣ�
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override{
        Vector3f wiLocal = normalize(toLocal(wi));          //��ɾֲ��ռ�

        //Ϊʲô���cos����Ϊ����  ����  ƽ��ķ����������棬wi��������һ����Ŀռ���
        return {abs(albedo[0] * INV_PI * wiLocal[1]),
                abs(albedo[1] * INV_PI * wiLocal[1]),
                abs(albedo[2] * INV_PI * wiLocal[1])};      // wiLocal[1]��ֵ����cos�ȵ�ֵ
    }

    //TODO��BSDF�ϲ���
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        //Lambert���ʳ��䷽�������䷽���޹أ���˴����wo���Բ���
        
        //CosineȨ�ز���
        Vector3f wi = squareToCosineHemisphere(sample);     //���ص��Ǿֲ������µ�
        float pdf = squareToCosineHemispherePdf(wi);
        
        //���Ȳ���
        // Vector3f wi = squareToUniformHemisphere(sample);
        // float pdf = squareToUniformHemispherePdf(wi);
        
        return {albedo, toWorld(wi), pdf, BSDFType::Diffuse};
    }

private:
    Spectrum albedo;    //������(����������ɫ��Ϣ)
};
