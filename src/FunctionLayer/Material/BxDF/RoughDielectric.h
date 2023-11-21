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
        Vector3f normal{0.f, 1.f, 0.f};

        //****  时刻保持wo,wh,n三者在同一空间区域内  ****

        //这里的eta：wi侧折射率/wo侧折射率。初始等于物体折射率/外界折射率（摄像机发出光线第一次折射肯定是由物体内部的wi产生的）
        Vector3f eta = eta_, eta_inv = eta_inv_;
        Vector3f n = normal;           //n表示当次反射/折射对应的宏观法线，初始让n指向物体外部
        //如果wo在物体内部，应该颠倒eta，让wi侧/wo侧等于外界折射率/物体折射率，并让n指向物体内部
        if(dot(wo_local, normal) < .0f){
            eta_inv = eta_;
            eta = eta_inv_;
            n = -normal;
        }

        //判断是反射还是折射
        bool isReflect = wo_local[1] * wi_local[1] > .0f;   //入射光线和出射光线是否同一侧空间
        //对折射的wh的计算见 https://zhuanlan.zhihu.com/p/459557696 中2.1上面那个地方，因为单位化，所以可以直接用相对eta算
        //计算wh，并使wh与wo同侧，此时n也与wo同侧
        Vector3f wh_local = isReflect ? normalize(wo_local + wi_local) : -normalize(eta[0] * wi_local - wo_local);


        // // 2. 根据公式计算 Fr, D, G
        float D = ndf->getD(wh_local, n, alpha);            //计算有多大比例的微表面法线朝向wh
        float Fr = getFr(dot(wh_local, wo_local));

        if(isReflect){
            float G = ndf->getG(wo_local, n, wh_local, alpha) * ndf->getG(wi_local, n, wh_local, alpha);
            // 注: brdf中分母的cos(θi)项与渲染方程中的cos项相消，因此分母只有4cos(θo)
            return albedo * D * G * Fr / (4 * dot(n, wo_local));
        }
        else{
            //找BTDF公式去   https://zhuanlan.zhihu.com/p/459557696
            //G这边传入的前三者是否需要在同一个空间内？
            float G = ndf->getG(wo_local, n, wh_local, alpha) * ndf->getG(wi_local, n, wh_local, alpha);
            return albedo * (1.f - Fr) * G * D
                   * (std::abs(dot(wh_local, wo_local) * dot(wh_local, wi_local)))
                   / (std::abs(dot(wo_local, n) * dot(wi_local,n))) 
                   / std::pow(eta_inv[0] * dot(wh_local, -wo_local) + dot(wh_local, wi_local), 2)
                   * dot(-n, wi_local) * eta_inv[0] * eta_inv[0];     //别忘了乘上cos项和变换立体角范围
        }
    }

    //Fr的计算与是conductor还是dielectric有关(这里用的是eta_，是材质折射率/外界折射率)
    float getFr(float cos_theta) const {
        float r0 = (eta_[0] - 1.f) * (eta_[0] - 1.f) /
                   ((eta_[0] + 1.f) * (eta_[0] + 1.f));
        return r0 + (1.f - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const std::shared_ptr<Sampler> &sampler) const override{
        Vector3f normal{0.f, 1.f, 0.f};
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(alpha, sampler->sampler2D());      //获取采样的whLocal(默认指向物体外部一侧)
        
        // //这里的eta：wi侧折射率/wo侧折射率。初始等于物体折射率/外界折射率（摄像机发出光线第一次折射肯定是由物体内部的wi产生的）
        Vector3f eta = eta_, eta_inv = eta_inv_;
        Vector3f n = normal;           //初始让n指向物体外部（本地空间下的）
        
        // //***  先让n,whlocal与wolocal的方向一致  ***

        //如果wo和normal反向，那么如果发生折射，wi在外界，wo在物体内部，修改eta为外界折射率/物体折射率
        if(dot(woLocal, normal) < .0f){
            n = -normal;        //修改n与wo同侧
            whLocal = -whLocal;     
            eta = eta_inv_;
            eta_inv = eta_;
        }

        float Fr = getFr(dot(whLocal, woLocal));    //两者一定同侧

        //判断是否可能发生折射，即是否为全反射(TODO，待修改)
        bool full_reflect = !ndf->refract(woLocal, whLocal, eta_inv);

        //如果只能发生全反射或者随机数小于fr，则抽样反射光线
        if(full_reflect || sampler->sampler1D() < Fr){
            // 此时，n, wolocal, whlocal和wilocal全在同一侧
            Vector3f wiLocal = ndf->calcWiLocalReflect(woLocal, whLocal);            //计算如果是反射的wi
            float pdf = Fr * ndf->getPDF_reflect_global(wiLocal, whLocal, n, alpha);    //已经转换为宏观半球上的pdf

            //pdf太小的话，在分母上时会导致结果非常大，从而带来问题
            //pdf太小直接不考虑，只采用pdf大的，将权重设置为0，然后在pathIntegrator中直接退出（暂时解决蓝色噪声问题，见日志）
            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Diffuse};
            
            //权重为什么这样分见日志
            return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Diffuse};     
        }
        else{   //否则抽样折射光线
            Vector3f wiLocal = ndf->calcWiLocalRefract(woLocal, whLocal, eta_inv);

            float pdf = (1.f - Fr) * ndf->getPDF_refract_global(woLocal, wiLocal, whLocal, n, alpha, eta_inv);

            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Refract};

            return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Refract};
        }
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta_;              //物体折射率/外界折射率
    Vector3f eta_inv_;          //外界折射率/物体折射率
    std::shared_ptr<NDF> ndf;

    // mutable bool inside = false;    //当前采样的入射光线是否在物体内部
};