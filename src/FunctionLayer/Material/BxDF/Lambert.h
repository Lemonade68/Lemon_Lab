#pragma once

//Lambert漫反射模型与Oren-Nayar漫反射模型区别：https://zhuanlan.zhihu.com/p/500809166
//Lambert漫反射模型为理想光滑平面

#include"BSDF.h"
#include"Sampling.h"

class Lambert_Reflection : public BSDF{
public:
    Lambert_Reflection(const Vector3f &_normal, const Vector3f &_tangent,
            const Vector3f &_bitangent, const Spectrum &_albedo)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo) {}

    //微表面散射模型中：Lambert材质对应的BSDF值（各个方向都相同，为常量*cosθ）
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override{
        Vector3f wiLocal = normalize(toLocal(wi));          //变成局部空间

        //为什么这边cos可能为负？  ——  平面的法线有两个面，wi可能在另一个面的空间中
        return {abs(albedo[0] * INV_PI * wiLocal[1]),
                abs(albedo[1] * INV_PI * wiLocal[1]),
                abs(albedo[2] * INV_PI * wiLocal[1])};      // wiLocal[1]的值就是cosθ的值
    }

    //TODO：BSDF上采样
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const std::shared_ptr<Sampler> &sampler) const override{
        //Lambert材质出射方向与入射方向无关，因此传入的wo可以不用
        
        //Cosine权重采样（就是漫反射的重要性采样）
        Vector3f wiLocal = squareToCosineHemisphere(sampler->sampler2D());     //返回的是局部坐标下的
        float pdf = squareToCosineHemispherePdf(wiLocal);
        
        //均匀采样
        // Vector3f wi = squareToUniformHemisphere(sample);
        // float pdf = squareToUniformHemispherePdf(wi);

        return {f(wo,toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Diffuse};
    }

private:
    Spectrum albedo;    //反射率(包含纹理颜色信息)
};
