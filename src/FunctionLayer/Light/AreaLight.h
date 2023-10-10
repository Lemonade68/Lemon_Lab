#pragma once

#include"Light.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Parallelogram.h"

class AreaLight : public Light{
public:
    //指定光强和形状，默认为平行四边形(头顶的)
    AreaLight(const Spectrum &_energy = Spectrum(1.f),
              const std::shared_ptr<Shape> &ptr = std::make_shared<Parallelogram>(Point3f(-1.f, 3.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(2.f, .0f, .0f)));

    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const override;

public:
    std::shared_ptr<Shape> shape;
    Spectrum energy;
};
