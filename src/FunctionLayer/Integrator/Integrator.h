#pragma once

#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Sampler/Sampler.h"
#include<vector>
#include<memory>

class Integrator{
public:
    Integrator() = default;
    virtual ~Integrator() = default;

    //�������scene�£�����������Ĺ��߷��ص����ս��ֵ
    virtual Spectrum li(Ray &ray, const std::vector<std::shared_ptr<Shape>> &scene,
                        std::shared_ptr<Sampler> sampler) const = 0;
};

//TODO����BSDF����ת�����Թ�Դ����