#pragma once

#include"Light.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Parallelogram.h"

class AreaLight : public Light{
public:
    //ָ����ǿ����״��Ĭ��Ϊƽ���ı���(ͷ����)
    AreaLight(const std::shared_ptr<Shape> &ptr = std::make_shared<Parallelogram>(Point3f(-1.f, 3.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(2.f, .0f, .0f)),
              const Spectrum &_energy = Spectrum(5.f));

    //Ҫ��״��ɶ��  ��Դ�ϲ���ʱ��Ҫ֪����Դ����״

    virtual Spectrum evaluateEmission(const Intersection &intersection, const Vector3f &wo) const override;

    //�����Դ�ϲ���һ���㣬���ز�����Ϣ
    virtual LightSampleResult sample(const Intersection &shadingPoint, const Vector2f &sample) const override;

    virtual void debugPrint() const{
        printf("<Light> shape: ");
        shape->debugPrint();
    }

public:
    std::shared_ptr<Shape> shape;
    Spectrum energy;
};
