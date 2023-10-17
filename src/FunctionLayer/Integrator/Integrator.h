#pragma once

#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
// #include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Sampler/Sampler.h"
#include"FunctionLayer/Scene/Scene.h"
#include<vector>
#include<memory>

class Integrator{
public:
    Integrator() = default;
    virtual ~Integrator() = default;

    //计算给定scene下，摄像机发出的光线返回的最终结果值
    virtual Spectrum li(Ray &ray, const Scene &scene,
                        std::shared_ptr<Sampler> sampler) const = 0;
};

//TODO：对BSDF采样转化到对光源采样
inline float convertPDF(const LightSampleResult &result){
    float pdf = result.pdf;
    float distance = result.distance;

    switch(result.type){
        case LightType::SpotLight:
            pdf *= distance * distance;     //考虑距离衰减：其实是乘1/r方，但pdf在分母上，因此直接相乘
            break;
        case LightType::AreaLight:
            pdf *= distance * distance;
            pdf /= std::abs(dot(result.normal, result.direction));     //这里是把cosθ'移到了分母上，因此变成除以，公式同101
            break;
        case LightType::EnvironmentLight:
            break;                          //* 环境光的pdf转换在采样时已经完成(什么意思待思考)
    }
    return pdf;
}