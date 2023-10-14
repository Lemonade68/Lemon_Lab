#pragma once

#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Intersection.h"

//枚举类enum class：只有使用LightType::SpotLight才行，不能直接使用SpotLight
enum class LightType {
    SpotLight,
    AreaLight,
    EnvironmentLight
};

//TODO：思考为什么需要LightSampleResult？
//光源上采样一个点，供BSDF采样光源使用
struct LightSampleResult{
    Spectrum energy;        //光源能量
    Vector3f direction;     //采样点相对于shading point的方向
    float distance;         //采样点相对于shading point的距离
    float pdf;              //采样到该点的pdf
    LightType type;         //光源类型
    bool isDelta;           //采样是否是一个delta分布
    Vector3f normal;        //采样点的法线（可选）
};

class Light{
public:
    Light() {}

    //当一条光线击中光源时，返回光源沿光线方向释放的radiance
    //这里是按照实际光线走的顺序来命名的，因此是出射光线wo的方向
    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const = 0;

    //从光源上采样一个点，返回采样的信息
    virtual LightSampleResult sample(const Intersection &shadingPoint, const Vector2f &sampler) const = 0;

    virtual void debugPrint() const = 0;

public:
    LightType type;
};


//无穷远处光源？
