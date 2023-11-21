#pragma once
#include"NDF.h"

//���幫ʽ�ɼ��ϴ�lab02 �� ֪��
class Beckmann_ndf : public NDF{
public:
    Beckmann_ndf() noexcept = default;
    virtual ~Beckmann_ndf() noexcept = default;

    virtual float getD(const Vector3f &whLocal, const Vector3f &normal, float alpha) const noexcept override{
        float theta = std::acos(dot(whLocal, normal));
        float D = exp(-tan(theta) * tan(theta) / (alpha * alpha)) / (PI * alpha * alpha * pow(cos(theta), 4));
        return D;
    }

    // ������㷨��ggx���㷨(��������Ч���õ�)
    // �ο����㷨 https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/utils/math.cuh#L184
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept override{
        const float cosTheta = dot(vLocal, normal);
        if(cosTheta * dot(vLocal,whLocal) <= .0f)
            return .0f;

        const float cosTheta2 = cosTheta * cosTheta,
                    tanTheta2 = (1.f - cosTheta2) / cosTheta2,
                    alpha2 = alpha * alpha;

        return 2.f / (1.f + sqrt(1.f + alpha2 * tanTheta2));

        // ����beckmann���㷨
        // float theta_v = std::acos(dot(vLocal, whLocal));
        // float a = 1.f / (alpha * std::tan(theta_v));
        // float G = (a < 1.6f) ? ((3.535f * a + 2.181f * a * a) / (1.f + 2.276f * a + 2.577f * a * a)) : 1.f;
        // return G;
    }

    // beckmann����wh�ķ���������ʹ��inversion�������в���
    // https://zhuanlan.zhihu.com/p/408360008 �Լ� moer-lite
    virtual Vector3f sampleWh(const float &alpha, const Vector2f &sample) const noexcept override {
        float logSample = std::log(1 - sample[0]);
        float phi = 2.f * PI * sample[1];       //�յ�ֵ
        float tanTheta2 = -alpha * alpha * logSample;

        float cosTheta = 1.f / std::sqrt(1.f + tanTheta2);
        float sinTheta = sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        Vector3f whLocal{sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi)};
        return whLocal;
    }

};