#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //˼��������ʲô

    //�ο�moer-lite��ֱ�ӽ��Է��߷ֲ�����D����Ӱ�ڱ���G�ļ���ȫ������NDF��
    //�����ߵļ�������ض���ndf�����й�
    virtual float getD(const Vector3f &whLocal, const Vector3f &normal, float alpha) const noexcept = 0;
    //ע��G��h�ķ����йأ�����Ҫ����whlocal��ֵ
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept = 0;

    //����΢����ģ�ͣ���Ҫ�Բ����������Ƿ��ߵķ���wh�Լ���pdf����Ҫ�ں������ת��
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  ��  https://zhuanlan.zhihu.com/p/475748352
    // sample��������Ĭ��normal��ָ�������ⲿ��һ���wh
    virtual Vector3f sampleWh(const float &alpha, const Vector2f &sampler) const noexcept = 0;
    
    //���㷴���pdf������ռ��£�
    //ע����������������wi��wo�������������Ķ��Ƿ������ģ�
    float getPDF_reflect_global(const Vector3f &wiLocal, const Vector3f &whLocal, const Vector3f &normal, const float &alpha) const noexcept{
        // https://zhuanlan.zhihu.com/p/475748352 �п�֪��wh��pdf����D�ٳ���cos��
        float pdf = getD(whLocal, normal, alpha) * std::abs(whLocal[1]);      //wh��normal��Զ��ͬһ����

        //�õ����������΢���淨��h�İ���������Ԫ�������Ǻ�۱��淨��ȷ���İ��������Ҫ�����ſɱȾ�������ʽ��ת��
        pdf *= 1.f / (4.f * std::abs(dot(wiLocal, whLocal)));
        return pdf;
    }

    float getPDF_refract_global(const Vector3f &woLocal, const Vector3f &wiLocal, const Vector3f &whLocal, const Vector3f &normal, const float &alpha, const Vector3f &inv_eta) const noexcept{
        float pdf = getD(whLocal, normal, alpha) * std::abs(whLocal[1]);
        //����pdf���ſɱ�����ʽת�����������������ӣ�ע���������wi/wo�෴����wo�ķ����������wi�෴��
        pdf *= std::abs(dot(wiLocal, whLocal)) /
               (std::pow((inv_eta[0] * dot(-woLocal, whLocal) + dot(wiLocal, whLocal)), 2));
        return pdf;
    }

    //============Ϊ�˷��㣬��ndf��Ա�м����������������㷴�������ĺ���============

    //����wh��wo����wi�����淴�䣩
    Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //���õ���������(������������Ϊ1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }

    //����wh��wo����wi�����䣩��ע���ʱwh��woͬ��
    // https://blog.csdn.net/yinhun2012/article/details/79472364�� ���н�-wo��Ϊ���䷽�򣨹�·���棩
    Vector3f calcWiLocalRefract(const Vector3f &woLocal, const Vector3f &whLocal, const Vector3f &eta_inv){
        return - std::sqrt(1.f - eta_inv[0] * eta_inv[0] * (1.f - dot(-woLocal, whLocal) * dot(-woLocal, whLocal))) * whLocal 
               + std::abs(eta_inv[0]) * (-woLocal + std::abs(dot(-woLocal, whLocal)) * whLocal);
    }

    //�ж��Ƿ���ܷ������䣬��Ϊfalse��ʾֻ�ܷ���ȫ����
    // https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/renderer/ray.cu#L469
    bool refract(const Vector3f &woLocal, const Vector3f &whLocal, const Vector3f &eta_inv){
        //sin��i = sin��o * no/ni��ֻҪ��i�޽��˵��ֻ��ȫ����
        //���øú���ʱ��wolocal,normal,whlocalλ��ͬһ�ռ���
        float cosThetaO = std::abs(dot(woLocal, whLocal));
        float cosThetaO2 = cosThetaO * cosThetaO;
        float sinThetaO = std::sqrt(1.f - cosThetaO2);
        float sinThetaI = sinThetaO * eta_inv[0];
        if(sinThetaI <.0f || sinThetaI > 1.f)
            return false;       //������������
        return true;
    }
};
