#pragma once

#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Intersection.h"

//ö����enum class��ֻ��ʹ��LightType::SpotLight���У�����ֱ��ʹ��SpotLight
enum class LightType {
    SpotLight,
    AreaLight,
    EnvironmentLight
};

//TODO��˼��Ϊʲô��ҪLightSampleResult��

class Light{
public:
    Light() {}

    //��һ�����߻��й�Դʱ�����ع�Դ�ع��߷����ͷŵ�radiance
    //�����ǰ���ʵ�ʹ����ߵ�˳���������ģ�����ǳ������wo�ķ���
    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const = 0;


public:
    LightType type;
};


//����Զ����Դ��
