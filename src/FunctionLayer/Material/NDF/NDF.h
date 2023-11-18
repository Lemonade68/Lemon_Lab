#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //思考作用是什么

    //参考moer-lite，直接将对法线分布函数D、阴影遮蔽项G的计算全部放在NDF中
    //这两者的计算均与特定的ndf种类有关
    virtual float getD(const Vector3f &whLocal, float alpha) const noexcept = 0;
    virtual float getG(const Vector3f &woLocal, const Vector3f &wiLocal, float alpha) const noexcept = 0;

    //对于微表面模型，重要性采样采样的是法线的方向wh以及其pdf，需要在后面进行转换
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  和  https://zhuanlan.zhihu.com/p/475748352
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sampler) const noexcept = 0;
    virtual float getPDF_reflect_global(const Vector3f &woLocal, const Vector3f &whLocal, const float &alpha) const noexcept = 0;
    //根据wh和wo计算wi（镜面反射）
    virtual Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //利用等腰三角形(方向向量长度为1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }
};
