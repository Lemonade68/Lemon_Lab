#include"BSDF.h"

class Specular_Reflection : public BSDF{
public:
    Specular_Reflection(const Vector3f &_normal, const Vector3f &_tangent, const Vector3f &_bitangent, const Spectrum &_color)
        : BSDF(_normal, _tangent, _bitangent), color(_color) {}
    
    //完全镜面反射直接采样到有效值的概率为0  ——  只有那一条光线有信息(本次的贡献为零，继续添加下次的贡献)
    //sampleLight方法不是都会变成0了？
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override{
        return Spectrum(.0f);
    }

    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const std::shared_ptr<Sampler> &sampler) const override{
        Vector3f woLocal = normalize(toLocal(wo));
        Vector3f wiLocal(-woLocal[0], woLocal[1], -woLocal[2]);     //x z反转，y不变
        return {color * Spectrum(1.f), toWorld(wiLocal), 1.f, BSDFType::Specular};      //出射光线唯一，因此pdf直接为1
    }

private:
    Spectrum color;     //材质本身的颜色
};