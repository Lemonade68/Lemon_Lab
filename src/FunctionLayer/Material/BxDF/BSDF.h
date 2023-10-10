#pragma once
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Shape/Intersection.h"

enum class BSDFType {
    Diffuse,
    Specular
};


class BSDF{
public:
    BSDF(const Vector3f &_normal, const Vector3f &_tangent,
         const Vector3f &_bitangent){
        normal = _normal;
        tangent = _tangent;
        bitangent = _bitangent;
    }

    //计算wi和wo对应的BSDF值(公式中的F项)
    virtual Spectrum F(const Vector3f &wo, const Vector3f &wi) const = 0;

    //TODO：在BSDF上采样的方法
    

public:
    //构成该点下的局部坐标系(这三是世界坐标系下的值)(应该是单位基向量)
    Vector3f normal, tangent, bitangent;

protected:
    //世界空间向量变成局部空间：直接投影到各个坐标轴上即可
    Vector3f toLocal(const Vector3f &world) const{
        return Vector3f{dot(world, tangent), dot(world, normal), dot(world, bitangent)};
    }

    //局部空间向量变回世界空间：乘上基向量在世界空间的表示
    Vector3f toWorld(const Vector3f &local) const{
        return local.x() * tangent + local.y() * normal + local.z() * bitangent;
    }
};