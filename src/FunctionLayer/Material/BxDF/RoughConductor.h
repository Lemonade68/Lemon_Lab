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
        // 1. 转换坐标系到局部坐标
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        Vector3f wh_local = normalize(wo_local + wi_local);    //半程向量（微观法线）
        Vector3f normal{0.f, 1.f, 0.f};

        // 2. 根据公式计算 Fr, D, G
        Vector3f Fr = getFr(dot(wh_local, wo_local));
        float D = ndf->getD(wh_local, alpha);
        float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(wi_local, normal, wh_local, alpha);

        // 注: brdf中分母的cos(θi)项与渲染方程中的cos项相消，因此分母只有4cos(θo)
        return albedo * D * G * Fr / (4 * dot(normal, wo_local));
    }

    //Fr的计算与是conductor还是dielectric有关
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = ((eta - 1.f) * (eta - 1.f) + eta_k * eta_k) /
                      ((eta + 1.f) * (eta + 1.f) + eta_k * eta_k);
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    //TODO：使用inversion method来进行microfacet材质重要性采样
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        // Vector3f wi = squareToCosineHemisphere(sample);
        // float pdf = squareToCosineHemispherePdf(wi);
        
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(woLocal, alpha, sample);   //获取采样的whLocal
        Vector3f wiLocal = ndf->calcWiLocalReflect(woLocal, whLocal);    //通过出射光线和法线计算入射光线（镜面反射）
        float pdf = ndf->getPDF_reflect_global(wiLocal, whLocal, alpha);    //已经转换为宏观半球上的pdf

        //pdf太小的话，在分母上时会导致结果非常大，从而带来问题
        //pdf太小直接不考虑，只采用pdf大的，将权重设置为0，然后在pathIntegrator中直接退出（暂时解决蓝色噪声问题，见日志）
        if(pdf < 1e-2f)     
            return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Diffuse};
        
        //这个权重怎么分配的？
        //见日志记录第22点（path integrator中权重的理解）
        return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Diffuse};     
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta;
    Vector3f eta_k;
    std::shared_ptr<NDF> ndf;
};