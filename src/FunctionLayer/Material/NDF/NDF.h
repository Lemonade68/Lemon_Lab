#pragma once

#include"CoreLayer/Math/Math.h"

class NDF{
public:
    virtual ~NDF() noexcept = default;      //˼��������ʲô

    //�ο�moer-lite��ֱ�ӽ��Է��߷ֲ�����D����Ӱ�ڱ���G�ļ���ȫ������NDF��
    //�����ߵļ�������ض���ndf�����й�
    virtual float getD(const Vector3f &whLocal, float alpha) const noexcept = 0;
    //ע��G��h�ķ����йأ�����Ҫ����whlocal��ֵ
    virtual float getG(const Vector3f &vLocal, const Vector3f &normal, const Vector3f &whLocal, float alpha) const noexcept = 0;

    //����΢����ģ�ͣ���Ҫ�Բ����������Ƿ��ߵķ���wh�Լ���pdf����Ҫ�ں������ת��
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/  ��  https://zhuanlan.zhihu.com/p/475748352
    virtual Vector3f sampleWh(const Vector3f &woLocal, const float &alpha, const Vector2f &sampler) const noexcept = 0;
    
    //���㷴���pdf������ռ��£�
    //ע����������������wi��wo�������������Ķ��Ƿ������ģ�
    float getPDF_reflect_global(const Vector3f &wiLocal, const Vector3f &whLocal, const float &alpha) const noexcept{
        // https://zhuanlan.zhihu.com/p/475748352 �п�֪��wh��pdf����D�ٳ���cos��
        float pdf = getD(whLocal, alpha) * whLocal[1];

        //�õ����������΢���淨��h�İ���������Ԫ�������Ǻ�۱��淨��ȷ���İ��������Ҫ�����ſɱȾ�������ʽ��ת��
        pdf *= 1.f / (4.f * std::abs(dot(wiLocal, whLocal)));
        return pdf;
    }

    //���������pdf������ռ��£�������eta����������ʣ������/͸��ࣩ��inv���䵹��
    //Ϊ��һ���ԣ���ʱʹ��inv_eta�ĵ�һ����������Ϊconductor�Ǳ�eta�õ���vector����ʵ�о�ֱ����floatҲ�У�
    //ע�⣺������wi�ǹ������䷽��wo���������������ƺ��������Ƿ�������
    float getPDF_refract_global(const Vector3f &woLocal, const Vector3f &wiLocal, const Vector3f &whLocal, const float &alpha, const Vector3f &inv_eta) const noexcept{
        float pdf = getD(whLocal, alpha) * whLocal[1];
        //����pdf���ſɱ�����ʽת������������������
        pdf *= std::abs(dot(wiLocal, whLocal)) /
               ((inv_eta[0] * dot(woLocal, whLocal) + dot(wiLocal, whLocal)) * (inv_eta[0] * dot(woLocal, whLocal) + dot(wiLocal, whLocal)));
        return pdf;
    }

    //============Ϊ�˷��㣬��ndf��Ա�м����������������㷴�������ĺ���============

    //����wh��wo����wi�����淴�䣩
    Vector3f calcWiLocalReflect(const Vector3f &woLocal, const Vector3f &whLocal) const noexcept {
        //���õ���������(������������Ϊ1)
        return 2 * dot(woLocal, whLocal) * whLocal - woLocal;
    }

    //�ж��Ƿ���ȫ���䣬�����������¼������ߣ������eta��ʱ���ɣ�
    // https://github.com/zhiwei-c/Monte-Carlo-Path-Tracing/blob/main/src/renderer/ray.cu#L469
    bool refract(const Vector3f &woLocal, const Vector3f &normal, const Vector3f &eta, Vector3f *wiLocal){
        const float cosThetaI = std::abs(dot(woLocal, normal));
        const float k = 1.f - eta[0] * eta[0] * (1.f - cosThetaI * cosThetaI);
        if(k <.0f)      //����������
            return false;
        else{           //��������
            *wiLocal = normalize((eta[0] * woLocal + (eta[0] * cosThetaI - std::sqrt(k)) * normal));
            return true;
        }
    }
};
