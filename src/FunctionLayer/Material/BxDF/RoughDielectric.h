#pragma once
#include"../NDF/NDF.h"
#include"BSDF.h"
#include"Sampling.h"

class RoughDielectricBSDF : public BSDF{
public:
    RoughDielectricBSDF(const Vector3f &_normal, const Vector3f &_tangent,
                        const Vector3f &_bitangent, Spectrum _albedo,
                        float _alpha, Vector3f _eta, std::shared_ptr<NDF> _ndf)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo), alpha(_alpha),
          eta_(_eta), ndf(_ndf) { eta_inv_ = Vector3f(1.f / eta_[0]); }

    //wi�ǹ������䷽���ṩ��������wo�ǳ��䷽��
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override {
        // 1. ת������ϵ���ֲ�����
        Vector3f wo_local = normalize(toLocal(wo));
        Vector3f wi_local = normalize(toLocal(wi));
        
        Vector3f eta = eta_, eta_inv = eta_inv_;    //eta��͸���/�����
        //��������������ڲ���ͨ�������ڲ���inside�жϣ�insideͨ��sample���иı䣩��Ӧ�õߵ�������
        if(inside){
            Vector3f temp = eta_inv;
            eta_inv = eta;
            eta = temp;
        }

        //�ж��Ƿ��仹������
        bool isReflect = wo_local[1] * wi_local[1] > .0f;   //������ߺͳ�������Ƿ�ͬһ��ռ�
        //�������wh�ļ���� https://zhuanlan.zhihu.com/p/459557696 ��2.1�����Ǹ��ط�����Ϊ��λ�������Կ���ֱ�������eta��
        //����wh����ʹwh��woͬ��
        Vector3f wh_local = isReflect ? normalize(wo_local + wi_local) : -normalize(eta[0] * wo_local + wi_local);

        // 2. ���ݹ�ʽ���� Fr, D, G
        Vector3f Fr = getFr(dot(wh_local, wo_local));
        float D = ndf->getD(wh_local, alpha);

        if(isReflect){
            float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(wi_local, normal, wh_local, alpha);
            // ע: brdf�з�ĸ��cos(��i)������Ⱦ�����е�cos����������˷�ĸֻ��4cos(��o)
            return albedo * D * G * Fr / (4 * dot(normal, wo_local));
        }
        else{
            //���ɣ�wi�Ǳ��ǲ���Ҫ�ø���
            float G = ndf->getG(wo_local, normal, wh_local, alpha) * ndf->getG(-wi_local, normal, wh_local, alpha);
            return albedo * ((std::abs(dot(wh_local, wo_local)) * std::abs(dot(wh_local, wi_local))) * (Vector3f(1.f) - Fr) * G * D)
                   / std::abs(dot(wi_local, normal) * std::pow(eta[0] * dot(wh_local, wo_local) + dot(wh_local, wi_local), 2)) 
                   * dot(normal, wi_local) * eta_inv[0] * eta_inv[0];     //�����˳���cos��ͱ任����Ƿ�Χ
        }
    }

    //Fr�ļ�������conductor����dielectric�й�
    Vector3f getFr(float cos_theta) const {
        Vector3f r0 = (eta_ - 1.f) * (eta_ - 1.f) /
                      ((eta_ + 1.f) * (eta_ + 1.f));
        return r0 + (Vector3f(1.f) - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const Vector2f &sample) const override{
        Vector3f eta = eta_, eta_inv = eta_inv_;

        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(woLocal, alpha, sample);   //��ȡ������whLocal
        float D = ndf->getD(whLocal, alpha) / (alpha * alpha);
        Vector3f wi;
        //����������
        bool full_reflect = !ndf->refract(woLocal, normal, eta_inv, &wi);   //��������Ļ�������������߷���

        //���ɣ�Ϊʲô��wiLocal��
        Vector3f Fr = getFr(dot(whLocal, woLocal));

        if(!inside){
            Vector3f temp = eta_inv;
            eta_inv = eta;
            eta = temp;
        }

        if(full_reflect || sample[0] < Fr[0]){
            //�����������
            float pdf = ndf->getPDF_reflect_global(woLocal, whLocal, alpha);    //�Ѿ�ת��Ϊ��۰����ϵ�pdf
            wi = ndf->calcWiLocalReflect(woLocal, whLocal);    //ͨ��������ߺͷ��߼���������ߣ����淴�䣩

            //pdf̫С�Ļ����ڷ�ĸ��ʱ�ᵼ�½���ǳ��󣬴Ӷ���������
            //pdf̫Сֱ�Ӳ����ǣ�ֻ����pdf��ģ���Ȩ������Ϊ0��Ȼ����pathIntegrator��ֱ���˳�����ʱ�����ɫ�������⣬����־��
            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wi), pdf, BSDFType::Diffuse};
            
            //Ȩ��Ϊʲô�����ּ���־
            return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Diffuse};     
        }
        else{
            //�����������
            wi = -wi;
            //�ı�inside״̬������һ�μ���fʹ�ã�
            inside = !inside;

            float G = ndf->getG(wo, -normal, -whLocal, alpha) * ndf->getG(wi, normal, whLocal, alpha);

            //pdf�Ƿ���Ȼ��Ҫ�޸�
            // float pdf = ndf->getPDF_refract_global(woLocal, wi, whLocal, alpha, eta_inv);
            float pdf = (1.f - Fr[0]) * D * abs(dot(wi, whLocal) / pow(eta[0] * dot(whLocal, woLocal) + dot(whLocal, wi), 2));

            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wi), pdf, BSDFType::Refract};

            return {f(wo, toWorld(wi)) / pdf, toWorld(wi), pdf, BSDFType::Refract};
        }
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta_;          //�����ǲ���ģ�����ÿ�κ�����ʹ�õ�����Ҫ�仯
    Vector3f eta_inv_;
    std::shared_ptr<NDF> ndf;

    mutable bool inside = false;    //��ǰ��������������Ƿ��������ڲ�
};