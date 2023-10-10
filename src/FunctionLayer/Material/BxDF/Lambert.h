#pragma once

#include"BSDF.h"

class Lambert_Reflection : public BSDF{
public:
    Lambert_Reflection(const Vector3f &_normal, const Vector3f &_tangent,
            const Vector3f &_bitangent, const Spectrum &_albedo)
        : BSDF(_normal, _tangent, _bitangent), albedo(_albedo) {}

    //Lambert材质对应的BSDF值（各个方向都相同，为常量*cosθ）
    virtual Spectrum F(const Vector3f &wo, const Vector3f &wi) const override{
        Vector3f wiLocal = normalize(toLocal(wi));          //变成局部空间
        return albedo * INV_PI * wiLocal[1];                //wiLocal[1]的值就是cosθ的值
    }

    //TODO：BSDF上采样
    
private:
    Spectrum albedo;    //反射率
};
