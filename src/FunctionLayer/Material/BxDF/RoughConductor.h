#pragma once
#include"../NDF/NDF.h"
#include"BSDF.h"
#include"Sampling.h"

class RoughConductorBSDF : public BSDF{
public:
    RoughConductorBSDF(const Vector3f &_normal, const Vector3f &_tangent,
                       const Vector3f &_bitangent, Spectrum _albedo,
                       float _alpha, Vector3f _eta, Vector3f _k, std::shared_ptr<NDF> _ndf)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo), alpha(_alpha),
          eta(_eta), eta_k(_k), ndf(_ndf){}

    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override {
        // 1. ת������ϵ���ֲ�����
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        Vector3f wh_local = normalize(wo_local + wi_local);    //���������΢�۷��ߣ�
        Vector3f normal{0.f, 1.f, 0.f};

        // 2. ���ݹ�ʽ���� Fr, D, G
        Vector3f Fr = getFr(dot(wh_local, wo_local));
        float D = ndf->getD(wh_local, alpha);
        float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(wi_local, normal, wh_local, alpha);

        // ע: brdf�з�ĸ��cos(��i)������Ⱦ�����е�cos����������˷�ĸֻ��4cos(��o)
        return albedo * D * G * Fr / (4 * dot(normal, wo_local));
    }

    //Fr�ļ�������conductor����dielectric�й�
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = ((eta - 1.f) * (eta - 1.f) + eta_k * eta_k) /
                      ((eta + 1.f) * (eta + 1.f) + eta_k * eta_k);
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    //TODO��ʹ��inversion method������microfacet������Ҫ�Բ���
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        // Vector3f wi = squareToCosineHemisphere(sample);
        // float pdf = squareToCosineHemispherePdf(wi);
        
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(woLocal, alpha, sample);   //��ȡ������whLocal
        Vector3f wiLocal = ndf->calcWiLocalReflect(woLocal, whLocal);    //ͨ��������ߺͷ��߼���������ߣ����淴�䣩
        float pdf = ndf->getPDF_reflect_global(wiLocal, whLocal, alpha);    //�Ѿ�ת��Ϊ��۰����ϵ�pdf

        //pdf̫С�Ļ����ڷ�ĸ��ʱ�ᵼ�½���ǳ��󣬴Ӷ���������
        //pdf̫Сֱ�Ӳ����ǣ�ֻ����pdf��ģ���Ȩ������Ϊ0��Ȼ����pathIntegrator��ֱ���˳�����ʱ�����ɫ�������⣬����־��
        if(pdf < 1e-2f)     
            return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Diffuse};
        
        //���Ȩ����ô����ģ�
        //����־��¼��22�㣨path integrator��Ȩ�ص���⣩
        return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Diffuse};     
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta;
    Vector3f eta_k;
    std::shared_ptr<NDF> ndf;
};