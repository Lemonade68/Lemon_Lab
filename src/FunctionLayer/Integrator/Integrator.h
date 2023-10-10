#pragma once

#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Shape.h"
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