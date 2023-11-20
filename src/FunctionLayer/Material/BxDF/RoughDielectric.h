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
          eta_(_eta), ndf(_ndf) { eta_inv_ = Vector3f(1.f / eta_[0]); }

    //wi是光线入射方向（提供能量），wo是出射方向
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override {
        // 1. 转换坐标系到局部坐标
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        
        Vector3f eta = eta_, eta_inv = eta_inv_;    //eta：透射侧/入射侧
        //如果光线在物体内部（通过材质内部的inside判断，inside通过sample进行改变），应该颠倒折射率
        if(inside){
            Vector3f temp = eta_inv;
            eta_inv = eta;
            eta = temp;
        }

        //判断是反射还是折射
        bool isReflect = wo_local[1] * wi_local[1] > .0f;   //入射光线和出射光线是否同一侧空间
        //对折射的wh的计算见 https://zhuanlan.zhihu.com/p/459557696 中2.1上面那个地方，因为单位化，所以可以直接用相对eta算
        //计算wh，并使wh与wo同侧
        Vector3f wh_local = isReflect ? normalize(wo_local + wi_local) : -normalize(eta[0] * wo_local + wi_local);

        // 2. 根据公式计算 Fr, D, G
        Vector3f Fr = getFr(dot(wh_local, wo_local));
        float D = ndf->getD(wh_local, alpha);

        if(isReflect){
            float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(wi_local, normal, wh_local, alpha);
            // 注: brdf中分母的cos(θi)项与渲染方程中的cos项相消，因此分母只有4cos(θo)
            return albedo * D * G * Fr / (4 * dot(normal, wo_local));
        }
        else{
            //存疑：wi那边是不是要用负的
            float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(-wi_local, normal, wh_local, alpha);
            return albedo * ((std::abs(dot(wh_local, wo_local)) * std::abs(dot(wh_local, wi_local))) * (Vector3f(1.f) - Fr) * G * D)
                   / std::abs(dot(wi_local, normal) * std::pow(eta[0] * dot(wh_local, wo_local) + dot(wh_local, wi_local), 2)) 
                   * dot(normal, wi_local) * eta_inv[0] * eta_inv[0];     //别忘了乘上cos项和变换立体角范围
        }
    }

    //Fr的计算与是conductor还是dielectric有关
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = (eta_ - 1.f) * (eta_ - 1.f) /
                      ((eta_ + 1.f) * (eta_ + 1.f));
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        Vector3f eta = eta_, eta_inv = eta_inv_;

        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(woLocal, alpha, sample);   //获取采样的whLocal
        float D = ndf->getD(whLocal, alpha) / (alpha * alpha);
        Vector3f wi;
        //这里有问题
        bool full_reflect = !ndf->refract(woLocal, normal, eta_inv, &wi);   //可以折射的话计算了折射光线方向

        //存疑：为什么是wiLocal？
        Vector3f Fr = getFr(dot(whLocal, woLocal));

        if(!inside){
            Vector3f temp = eta_inv;
            eta_inv = eta;
            eta = temp;
        }

        if(full_reflect || sample[0] < Fr[0]){
            //抽样反射光线
            float pdf = ndf->getPDF_reflect_global(woLocal, whLocal, alpha);    //已经转换为宏观半球上的pdf
            wi = ndf->calcWiLocalReflect(woLocal, whLocal);    //通过出射光线和法线计算入射光线（镜面反射）

            //pdf太小的话，在分母上时会导致结果非常大，从而带来问题
            //pdf太小直接不考虑，只采用pdf大的，将权重设置为0，然后在pathIntegrator中直接退出（暂时解决蓝色噪声问题，见日志）
            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wi), pdf, BSDFType::Diffuse};
            
            //权重为什么这样分见日志
            return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Diffuse};     
        }
        else{
            //抽样折射光线
            wi = -wi;
            //改变inside状态（供下一次计算f使用）
            inside = !inside;

            float G = ndf->getG(wo, -normal, -whLocal, alpha) * ndf->getG(wi, normal, whLocal, alpha);

            //pdf是否仍然需要修改
            // float pdf = ndf->getPDF_refract_global(woLocal, wi, whLocal, alpha, eta_inv);
            float pdf = (1.f - Fr[0]) * D * abs(dot(wi, whLocal) / pow(eta[0] * dot(whLocal, woLocal) + dot(whLocal, wi), 2));

            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wi), pdf, BSDFType::Refract};

            return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Refract};
        }
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta_;          //这两是不变的，但是每次函数中使用的两个要变化
    Vector3f eta_inv_;
    std::shared_ptr<NDF> ndf;

    mutable bool inside = false;    //当前采样的入射光线是否在物体内部
};