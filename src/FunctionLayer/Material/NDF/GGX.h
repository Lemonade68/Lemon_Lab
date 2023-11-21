#pragma once
#include"NDF.h"

//具体公式可见南大lab02 或 知乎
class GGX : public NDF{
public:
    GGX() noexcept = default;
    virtual ~GGX() noexcept = default;

    virtual float getD(const Vector3f &whLocal, const Vector3f &normal, float alpha) const noexcept override{
        const float cos_theta = dot(normal, whLocal);
        if (cos_theta <= 0.0f)
            return 0.0f;
        const float cos_theta_2 = cos_theta * cos_theta,
                    tan_theta_2 = (1.0f - cos_theta_2) / cos_theta_2,
                    cos_theta_3 = pow(cos_theta, 3),
                    alpha_2 = alpha * alpha;
        return alpha_2 / (PI * cos_theta_3 * pow(alpha_2 + tan_theta_2, 2));
    }

    // 参考该算法 https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/utils/math.cuh#L184
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept override{
        const float cosTheta = dot(vLocal, normal);
        if(cosTheta * dot(vLocal,whLocal) <= .0f)
            return .0f;

        const float cosTheta2 = cosTheta * cosTheta,
                    tanTheta2 = (1.f - cosTheta2) / cosTheta2,
                    alpha2 = alpha * alpha;

        return 2.f / (1.f + sqrt(1.f + alpha2 * tanTheta2));
    }

    // ggx采样wh的方法：还是使用inversion方法进行采样
    // 注意我的坐标系中是y朝上
    // https://zhuanlan.zhihu.com/p/408360008 以及上面的渲染器
    virtual Vector3f sampleWh(const float &alpha, const Vector2f &sample) const noexcept override {
        const float phi = 2.0f * PI * sample[0],
                cos_phi = cosf(phi),
                sin_phi = sinf(phi),
                alpha_2 = alpha * alpha;
        const float tan_theta_2 = alpha_2 * sample[1] / (1.0f - sample[1]),
                    cos_theta = 1.0f / sqrt(1.0f + tan_theta_2),
                    sin_theta = sqrt(1.0f - cos_theta * cos_theta);
        Vector3f whLocal = Vector3f(sin_theta * cos_phi, cos_theta, sin_theta * sin_phi);
        return whLocal;
    }
};