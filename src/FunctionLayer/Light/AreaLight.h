#pragma once

#include"Light.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Parallelogram.h"

class AreaLight : public Light{
public:
    //指定光强和形状，默认为平行四边形(头顶的)
    AreaLight(const std::shared_ptr<Shape> &ptr = std::make_shared<Parallelogram>(Point3f(-1.f, 3.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(2.f, .0f, .0f)),
              const Spectrum &_energy = Spectrum(5.f));

    //要形状干啥？  光源上采样时需要知道光源的形状

    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const override;

    //在面光源上采样一个点，返回采样信息
    virtual LightSampleResult sample(const Intersection &shadingPoint, const Vector2f &sample) const override;

    virtual void debugPrint() const{
        printf("<Light> shape: ");
        shape->debugPrint();
    }

public:
    std::shared_ptr<Shape> shape;
    Spectrum energy;
};
