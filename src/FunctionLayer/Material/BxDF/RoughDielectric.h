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
        Vector3f normal{0.f, 1.f, 0.f};

        //****  ʱ�̱���wo,wh,n������ͬһ�ռ�������  ****

        //�����eta��wi��������/wo�������ʡ���ʼ��������������/��������ʣ�������������ߵ�һ������϶����������ڲ���wi�����ģ�
        Vector3f eta = eta_, eta_inv = eta_inv_;
        Vector3f n = normal;           //n��ʾ���η���/�����Ӧ�ĺ�۷��ߣ���ʼ��nָ�������ⲿ
        //���wo�������ڲ���Ӧ�õߵ�eta����wi��/wo��������������/���������ʣ�����nָ�������ڲ�
        if(dot(wo_local, normal) < .0f){
            eta_inv = eta_;
            eta = eta_inv_;
            n = -normal;
        }

        //�ж��Ƿ��仹������
        bool isReflect = wo_local[1] * wi_local[1] > .0f;   //������ߺͳ�������Ƿ�ͬһ��ռ�
        //�������wh�ļ���� https://zhuanlan.zhihu.com/p/459557696 ��2.1�����Ǹ��ط�����Ϊ��λ�������Կ���ֱ�������eta��
        //����wh����ʹwh��woͬ�࣬��ʱnҲ��woͬ��
        Vector3f wh_local = isReflect ? normalize(wo_local + wi_local) : -normalize(eta[0] * wi_local - wo_local);


        // // 2. ���ݹ�ʽ���� Fr, D, G
        float D = ndf->getD(wh_local, n, alpha);            //�����ж�������΢���淨�߳���wh
        float Fr = getFr(dot(wh_local, wo_local));

        if(isReflect){
            float G = ndf->getG(wo_local, n, wh_local, alpha) * ndf->getG(wi_local, n, wh_local, alpha);
            // ע: brdf�з�ĸ��cos(��i)������Ⱦ�����е�cos����������˷�ĸֻ��4cos(��o)
            return albedo * D * G * Fr / (4 * dot(n, wo_local));
        }
        else{
            //��BTDF��ʽȥ   https://zhuanlan.zhihu.com/p/459557696
            //G��ߴ����ǰ�����Ƿ���Ҫ��ͬһ���ռ��ڣ�
            float G = ndf->getG(wo_local, n, wh_local, alpha) * ndf->getG(wi_local, n, wh_local, alpha);
            return albedo * (1.f - Fr) * G * D
                   * (std::abs(dot(wh_local, wo_local) * dot(wh_local, wi_local)))
                   / (std::abs(dot(wo_local, n) * dot(wi_local,n))) 
                   / std::pow(eta_inv[0] * dot(wh_local, -wo_local) + dot(wh_local, wi_local), 2)
                   * dot(-n, wi_local) * eta_inv[0] * eta_inv[0];     //�����˳���cos��ͱ任����Ƿ�Χ
        }
    }

    //Fr�ļ�������conductor����dielectric�й�(�����õ���eta_���ǲ���������/���������)
    float getFr(float cos_theta) const {
        float r0 = (eta_[0] - 1.f) * (eta_[0] - 1.f) /
                   ((eta_[0] + 1.f) * (eta_[0] + 1.f));
        return r0 + (1.f - r0) * std::pow(1.f - cos_theta, 5.f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const std::shared_ptr<Sampler> &sampler) const override{
        Vector3f normal{0.f, 1.f, 0.f};
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f whLocal = ndf->sampleWh(alpha, sampler->sampler2D());      //��ȡ������whLocal(Ĭ��ָ�������ⲿһ��)
        
        // //�����eta��wi��������/wo�������ʡ���ʼ��������������/��������ʣ�������������ߵ�һ������϶����������ڲ���wi�����ģ�
        Vector3f eta = eta_, eta_inv = eta_inv_;
        Vector3f n = normal;           //��ʼ��nָ�������ⲿ�����ؿռ��µģ�
        
        // //***  ����n,whlocal��wolocal�ķ���һ��  ***

        //���wo��normal������ô����������䣬wi����磬wo�������ڲ����޸�etaΪ���������/����������
        if(dot(woLocal, normal) < .0f){
            n = -normal;        //�޸�n��woͬ��
            whLocal = -whLocal;     
            eta = eta_inv_;
            eta_inv = eta_;
        }

        float Fr = getFr(dot(whLocal, woLocal));    //����һ��ͬ��

        //�ж��Ƿ���ܷ������䣬���Ƿ�Ϊȫ����(TODO�����޸�)
        bool full_reflect = !ndf->refract(woLocal, whLocal, eta_inv);

        //���ֻ�ܷ���ȫ������������С��fr��������������
        if(full_reflect || sampler->sampler1D() < Fr){
            // ��ʱ��n, wolocal, whlocal��wilocalȫ��ͬһ��
            Vector3f wiLocal = ndf->calcWiLocalReflect(woLocal, whLocal);            //��������Ƿ����wi
            float pdf = Fr * ndf->getPDF_reflect_global(wiLocal, whLocal, n, alpha);    //�Ѿ�ת��Ϊ��۰����ϵ�pdf

            //pdf̫С�Ļ����ڷ�ĸ��ʱ�ᵼ�½���ǳ��󣬴Ӷ���������
            //pdf̫Сֱ�Ӳ����ǣ�ֻ����pdf��ģ���Ȩ������Ϊ0��Ȼ����pathIntegrator��ֱ���˳�����ʱ�����ɫ�������⣬����־��
            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Diffuse};
            
            //Ȩ��Ϊʲô�����ּ���־
            return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Diffuse};     
        }
        else{   //��������������
            Vector3f wiLocal = ndf->calcWiLocalRefract(woLocal, whLocal, eta_inv);

            float pdf = (1.f - Fr) * ndf->getPDF_refract_global(woLocal, wiLocal, whLocal, n, alpha, eta_inv);

            if(pdf < 1e-2f)     
                return {Spectrum(.0f), toWorld(wiLocal), pdf, BSDFType::Refract};

            return {f(wo, toWorld(wiLocal)) / pdf, toWorld(wiLocal), pdf, BSDFType::Refract};
        }
    }

private:
    Spectrum albedo;
    float alpha;
    Vector3f eta_;              //����������/���������
    Vector3f eta_inv_;          //���������/����������
    std::shared_ptr<NDF> ndf;

    // mutable bool inside = false;    //��ǰ��������������Ƿ��������ڲ�
};