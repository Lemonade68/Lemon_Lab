#pragma once
#include"../NDF/NDF.h"
#include"BSDF.h"
#include"Sampling.h"

class RoughDielectricBSDF : public BSDF{
public:
    RoughDielectricBSDF(const Vector3f &_normal, const Vector3f &_tangent,
                       const Vector3f &_bitangent, Spectrum _albedo,
                       float _alpha, Vector3f _eta, std::shared_ptr<NDF> _ndf)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo), alpha(_alpha),
          eta(_eta), ndf(_ndf){}

    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override {
        // 1. ת������ϵ���ֲ�����
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        Vector3f wh_local = normalize(wo_local + wi_local);    //���������΢�۷��ߣ�
        Vector3f normal{0.f, 1.f, 0.f};

        // 2. ���ݹ�ʽ���� Fr, D, G
        Vector3f Fr = getFr(dot(normal, wo_local));
        float D = ndf->getD(wh_local, alpha);
        float G = ndf->getG(wo_local, wi_local, alpha);

        // ע: brdf�з�ĸ��cos(��i)������Ⱦ�����е�cos����������˷�ĸֻ��4cos(��o)
        return albedo * D * G * Fr / (4 * dot(normal, wo_local));
    }

    //Fr�ļ�������conductor����dielectric�й�
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = (eta - 1.f) * (eta - 1.f) /
                      ((eta + 1.f) * (eta + 1.f));
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    //dielectric������ʱʹ��cosineȨ�ز�����Ҫ�ģ�
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        Vector3f wi = squareToCosineHemisphere(sample);
        float pdf = squareToCosineHemispherePdf(wi);
        //���Ȩ����ô����ģ�   �ο���moer-lite
        return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Diffuse};
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta;
    std::shared_ptr<NDF> ndf;
};