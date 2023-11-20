#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //思考作用是什么

    //参考moer-lite，直接将对法线分布函数D、阴影遮蔽项G的计算全部放在NDF中
    //这两者的计算均与特定的ndf种类有关
    virtual float getD(const Vector3f &whLocal, float alpha) const noexcept = 0;
    //注意G和h的方向有关，所以要传入whlocal的值
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept = 0;

    //对于微表面模型，重要性采样采样的是法线的方向wh以及其pdf，需要在后面进行转换
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  和  https://zhuanlan.zhihu.com/p/475748352
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sampler) const noexcept = 0;
    
    //计算反射的pdf（世界空间下）
    //注意这个链接里的所有wi和wo，与我这个里面的都是反过来的！
    float getPDF_reflect_global(const Vector3f &wiLocal, const Vector3f &whLocal, const float &alpha) const noexcept{
        // https://zhuanlan.zhihu.com/p/475748352 中可知：wh的pdf就是D再乘上cosθ
        float pdf = getD(whLocal, alpha) * whLocal[1];

        //得到的是相对于微表面法线h的半球的立体角元，而不是宏观表面法线确定的半球，因此需要乘以雅可比矩阵行列式来转换
        pdf *= 1.f / (4.f * std::abs(dot(wiLocal, whLocal)));
        return pdf;
    }

    //计算折射的pdf（世界空间下），这里eta是相对折射率（入射侧/透射侧），inv是其倒数
    //为了一致性，暂时使用inv_eta的第一个分量（因为conductor那边eta用的是vector，其实感觉直接用float也行）
    //注意：我这里wi是光线入射方向，wo是摄像机射出方向，似乎与他的是反过来的
    float getPDF_refract_global(const Vector3f &woLocal, const Vector3f &wiLocal, const Vector3f &whLocal, const float &alpha, const Vector3f &inv_eta) const noexcept{
        float pdf = getD(whLocal, alpha) * whLocal[1];
        //折射pdf的雅可比行列式转换，具体见上面的链接
        pdf *= std::abs(dot(wiLocal, whLocal)) /
               ((inv_eta[0] * dot(woLocal, whLocal) + dot(wiLocal, whLocal)) * (inv_eta[0] * dot(woLocal, whLocal) + dot(wiLocal, whLocal)));
        return pdf;
    }

    //============为了方便，在ndf成员中加入了如下两个计算反射和折射的函数============

    //根据wh和wo计算wi（镜面反射）
    Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //利用等腰三角形(方向向量长度为1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }

    //判断是否发生全反射，若有折射则记录折射光线，这里的eta暂时存疑？
    // https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/renderer/ray.cu#L469
    bool refract(const Vector3f &woLocal, const Vector3f &normal, const Vector3f &eta, Vector3f *wiLocal){
        const float cosThetaI = std::abs(dot(woLocal, normal));
        const float k = 1.f - eta[0] * eta[0] * (1.f - cosThetaI * cosThetaI);
        if(k <.0f)      //不可能折射
            return false;
        else{           //可能折射
            *wiLocal = normalize((eta[0] * woLocal + (eta[0] * cosThetaI - std::sqrt(k)) * normal));
            return true;
        }
    }
};
