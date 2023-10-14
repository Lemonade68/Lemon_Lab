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
//��Դ�ϲ���һ���㣬��BSDF������Դʹ��
struct LightSampleResult{
    Spectrum energy;        //��Դ����
    Vector3f direction;     //�����������shading point�ķ���
    float distance;         //�����������shading point�ľ���
    float pdf;              //�������õ��pdf
    LightType type;         //��Դ����
    bool isDelta;           //�����Ƿ���һ��delta�ֲ�
    Vector3f normal;        //������ķ��ߣ���ѡ��
};

class Light{
public:
    Light() {}

    //��һ�����߻��й�Դʱ�����ع�Դ�ع��߷����ͷŵ�radiance
    //�����ǰ���ʵ�ʹ����ߵ�˳���������ģ�����ǳ������wo�ķ���
    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const = 0;

    //�ӹ�Դ�ϲ���һ���㣬���ز�������Ϣ
    virtual LightSampleResult sample(const Intersection &shadingPoint, const Vector2f &sampler) const = 0;

    virtual void debugPrint() const = 0;

public:
    LightType type;
};


//����Զ����Դ��
