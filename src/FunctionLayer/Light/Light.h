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

class Light{
public:
    Light() {}

    //当一条光线击中光源时，返回光源沿光线方向释放的radiance
    //这里是按照实际光线走的顺序来命名的，因此是出射光线wo的方向
    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const = 0;


public:
    LightType type;
};


//无穷远处光源？
