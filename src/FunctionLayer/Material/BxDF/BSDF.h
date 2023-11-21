#pragma once
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Shape/Intersection.h"
#include"FunctionLayer/Sampler/IndependentSampler.h"

enum class BSDFType {
    Diffuse,
    Specular,
    Refract
};

struct BSDFSampleResult{
    Spectrum weight;        //反射率、折射率等权重
    Vector3f wi;            //采样得到的实际的入射方向
    float pdf;              //该shading point的概率密度函数
    BSDFType type;
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
    //可被用来计算直接光照部分的f，也可以用来计算weight中上次光线的系数部分
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;

    //TODO：在BSDF上采样的方法
    virtual BSDFSampleResult sampleShadingPoint(const Vector3f &wo, const std::shared_ptr<Sampler> &sampler) const = 0;

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
        return local[0] * tangent + local[1] * normal + local[2] * bitangent;
    }
};