#pragma once
#include"NDF.h"

//���幫ʽ�ɼ��ϴ�lab02 �� ֪��
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

    virtual float getG(const Vector3f &woLocal, const Vector3f &wiLocal, float alpha) const noexcept override{
        Vector3f whLocal = normalize(wiLocal + woLocal);
        float theta_vi = std::acos(dot(wiLocal, whLocal)), theta_vo = std::acos(dot(woLocal, whLocal));
        float a_i = 1 / (alpha * std::tan(theta_vi)), a_o = 1 / (alpha * std::tan(theta_vo));
        float G1 = (a_i < 1.6f) ? ((3.535f * a_i + 2.181f * a_i * a_i) / (1 + 2.276f * a_i + 2.577f * a_i * a_i)) : 1.f;
        float G2 = (a_o < 1.6f) ? ((3.535f * a_o + 2.181f * a_o * a_o) / (1 + 2.276f * a_o + 2.577f * a_o * a_o)) : 1.f;
        return G1 * G2;
    }

    // beckmann����wh�ķ���������ʹ��inversion�������в���
    // https://zhuanlan.zhihu.com/p/408360008 �Լ� moer-lite
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sample) const noexcept override {
        float logSample = std::log(1 - sample[0]);
        float phi = 2.f * PI * sample[1];       //�յ�ֵ
        float tanTheta2 = -alpha * alpha * logSample;

        float cosTheta = 1.f / std::sqrt(1.f + tanTheta2);
        float sinTheta = sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        Vector3f whLocal{sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi)};
        return whLocal;
    }

    virtual float getPDF_reflect_global(const Vector3f &woLocal, const Vector3f &whLocal, const float &alpha) const noexcept{
        // https://zhuanlan.zhihu.com/p/475748352 �п�֪��wh��pdf����D�ٳ���cos��
        float pdf = getD(whLocal, alpha) * whLocal[1];

        //�õ����������΢���淨��h�İ���������Ԫ�������Ǻ�۱��淨��ȷ���İ��������Ҫ�����ſɱȾ�������ʽ��ת��
        pdf = pdf * 1.f / (4.f * std::abs(dot(woLocal, whLocal)));
        return pdf;
    }


    
};