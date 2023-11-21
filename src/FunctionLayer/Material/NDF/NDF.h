#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //思考作用是什么

    //参考moer-lite，直接将对法线分布函数D、阴影遮蔽项G的计算全部放在NDF中
    //这两者的计算均与特定的ndf种类有关
    virtual float getD(const Vector3f &whLocal, const Vector3f &normal, float alpha) const noexcept = 0;
    //注意G和h的方向有关，所以要传入whlocal的值
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept = 0;

    //对于微表面模型，重要性采样采样的是法线的方向wh以及其pdf，需要在后面进行转换
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  和  https://zhuanlan.zhihu.com/p/475748352
    // sample出来的是默认normal（指向物体外部）一侧的wh
    virtual Vector3f sampleWh(const float &alpha, const Vector2f &sampler) const noexcept = 0;
    
    //计算反射的pdf（世界空间下）
    //注意这个链接里的所有wi和wo，与我这个里面的都是反过来的！
    float getPDF_reflect_global(const Vector3f &wiLocal, const Vector3f &whLocal, const Vector3f &normal, const float &alpha) const noexcept{
        // https://zhuanlan.zhihu.com/p/475748352 中可知：wh的pdf就是D再乘上cosθ
        float pdf = getD(whLocal, normal, alpha) * std::abs(whLocal[1]);      //wh和normal永远是同一方向

        //得到的是相对于微表面法线h的半球的立体角元，而不是宏观表面法线确定的半球，因此需要乘以雅可比矩阵行列式来转换
        pdf *= 1.f / (4.f * std::abs(dot(wiLocal, whLocal)));
        return pdf;
    }

    float getPDF_refract_global(const Vector3f &woLocal, const Vector3f &wiLocal, const Vector3f &whLocal, const Vector3f &normal, const float &alpha, const Vector3f &inv_eta) const noexcept{
        float pdf = getD(whLocal, normal, alpha) * std::abs(whLocal[1]);
        //折射pdf的雅可比行列式转换，具体见上面的链接（注意与材料里wi/wo相反，且wo的方向与材料中wi相反）
        pdf *= std::abs(dot(wiLocal, whLocal)) /
               (std::pow((inv_eta[0] * dot(-woLocal, whLocal) + dot(wiLocal, whLocal)), 2));
        return pdf;
    }

    //============为了方便，在ndf成员中加入了如下两个计算反射和折射的函数============

    //根据wh和wo计算wi（镜面反射）
    Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //利用等腰三角形(方向向量长度为1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }

    //根据wh和wo计算wi（折射），注意此时wh和wo同侧
    // https://blog.csdn.net/yinhun2012/article/details/79472364， 其中将-wo视为入射方向（光路可逆）
    Vector3f calcWiLocalRefract(const Vector3f &woLocal, const Vector3f &whLocal, const Vector3f &eta_inv){
        return - std::sqrt(1.f - eta_inv[0] * eta_inv[0] * (1.f - dot(-woLocal, whLocal) * dot(-woLocal, whLocal))) * whLocal 
               + std::abs(eta_inv[0]) * (-woLocal + std::abs(dot(-woLocal, whLocal)) * whLocal);
    }

    //判断是否可能发生折射，若为false表示只能发生全反射
    // https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/renderer/ray.cu#L469
    bool refract(const Vector3f &woLocal, const Vector3f &whLocal, const Vector3f &eta_inv){
        //sinθi = sinθo * no/ni，只要θi无解就说明只能全反射
        //调用该函数时，wolocal,normal,whlocal位于同一空间内
        float cosThetaO = std::abs(dot(woLocal, whLocal));
        float cosThetaO2 = cosThetaO * cosThetaO;
        float sinThetaO = std::sqrt(1.f - cosThetaO2);
        float sinThetaI = sinThetaO * eta_inv[0];
        if(sinThetaI <.0f || sinThetaI > 1.f)
            return false;       //发生不了折射
        return true;
    }
};
