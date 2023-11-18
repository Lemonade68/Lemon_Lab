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
        // 1. 转换坐标系到局部坐标
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        Vector3f wh_local = normalize(wo_local + wi_local);    //半程向量（微观法线）
        Vector3f normal{0.f, 1.f, 0.f};

        // 2. 根据公式计算 Fr, D, G
        Vector3f Fr = getFr(dot(normal, wo_local));
        float D = ndf->getD(wh_local, alpha);
        float G = ndf->getG(wo_local, wi_local, alpha);

        // 注: brdf中分母的cos(θi)项与渲染方程中的cos项相消，因此分母只有4cos(θo)
        return albedo * D * G * Fr / (4 * dot(normal, wo_local));
    }

    //Fr的计算与是conductor还是dielectric有关
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = (eta - 1.f) * (eta - 1.f) /
                      ((eta + 1.f) * (eta + 1.f));
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    //dielectric部分暂时使用cosine权重采样（要改）
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        Vector3f wi = squareToCosineHemisphere(sample);
        float pdf = squareToCosineHemispherePdf(wi);
        //这个权重怎么分配的？   参考的moer-lite
        return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Diffuse};
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta;
    std::shared_ptr<NDF> ndf;
};