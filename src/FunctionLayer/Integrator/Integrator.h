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

    //�������scene�£�����������Ĺ��߷��ص����ս��ֵ
    virtual Spectrum li(Ray &ray, const Scene &scene,
                        std::shared_ptr<Sampler> sampler) const = 0;
};

//TODO����BSDF����ת�����Թ�Դ����
inline float convertPDF(const LightSampleResult &result){
    float pdf = result.pdf;
    float distance = result.distance;

    switch(result.type){
        case LightType::SpotLight:
            pdf *= distance * distance;     //���Ǿ���˥������ʵ�ǳ�1/r������pdf�ڷ�ĸ�ϣ����ֱ�����
            break;
        case LightType::AreaLight:
            pdf *= distance * distance;
            pdf /= std::abs(dot(result.normal, result.direction));     //�����ǰ�cos��'�Ƶ��˷�ĸ�ϣ���˱�ɳ��ԣ���ʽͬ101
            break;
        case LightType::EnvironmentLight:
            break;                          //* �������pdfת���ڲ���ʱ�Ѿ����(ʲô��˼��˼��)
    }
    return pdf;
}