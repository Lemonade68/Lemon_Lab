#pragma once
#include"NDF.h"

//具体公式可见南大lab02 或 知乎
class Beckmann_ndf : public NDF{
public:
    Beckmann_ndf() noexcept = default;
    virtual ~Beckmann_ndf() noexcept = default;

    virtual float getD(const Vector3f &whLocal, float alpha) const noexcept override{
        Vector3f normal{.0f, 1.f, .0f};
        float theta = std::acos(dot(whLocal, normal));
        float D = exp(-tan(theta) * tan(theta) / (alpha * alpha)) / (PI * alpha * alpha * pow(cos(theta), 4));
        return D;
    }

    // 这里的算法是ggx的算法(后续再说)
    // 参考该算法 https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/utils/math.cuh#L184
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept override{
        const float cosTheta = dot(vLocal, normal);
        if(cosTheta * dot(vLocal,whLocal) <= .0f)
            return .0f;

        const float cosTheta2 = cosTheta * cosTheta,
                    tanTheta2 = (1.f - cosTheta2) / cosTheta2,
                    alpha2 = alpha * alpha;

        return 2.f / (1.f + sqrt(1.f + alpha2 * tanTheta2));

        //下面是老的算法
        // float theta_vi = std::acos(dot(wiLocal, whLocal)), theta_vo = std::acos(dot(woLocal, whLocal));
        // float a_i = 1 / (alpha * std::tan(theta_vi)), a_o = 1 / (alpha * std::tan(theta_vo));
        // float G1 = (a_i < 1.6f) ? ((3.535f * a_i + 2.181f * a_i * a_i) / (1 + 2.276f * a_i + 2.577f * a_i * a_i)) : 1.f;
        // float G2 = (a_o < 1.6f) ? ((3.535f * a_o + 2.181f * a_o * a_o) / (1 + 2.276f * a_o + 2.577f * a_o * a_o)) : 1.f;
        // return G1 * G2;
    }

    // beckmann采样wh的方法：还是使用inversion方法进行采样
    // https://zhuanlan.zhihu.com/p/408360008 以及 moer-lite
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sample) const noexcept override {
        float logSample = std::log(1 - sample[0]);
        float phi = 2.f * PI * sample[1];       //φ的值
        float tanTheta2 = -alpha * alpha * logSample;

        float cosTheta = 1.f / std::sqrt(1.f + tanTheta2);
        float sinTheta = sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        Vector3f whLocal{sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi)};
        return whLocal;
    }

};