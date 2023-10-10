#pragma once

#include"Intersection.h"
#include"CoreLayer/Math/Transform.h"
#include"FunctionLayer/Ray/Ray.h"

#include"FunctionLayer/Material/Matte.h"
#include<optional>

class Light;
class Material;

class Shape : public Transformable{
public:
    //Ĭ�Ϲ��캯������ƽ�ƣ������ţ�����ת
    Shape(const Vector3f &translate = Vector3f(.0f, .0f, .0f),
          const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
          const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
          float radian = .0f);

    //TODO��ʹ��json��ʼ��shape

    //�����������󽻺���
    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const = 0;

public:
    std::shared_ptr<Light> light;           //���������Ƿ񷢹⣨��̬��ʹ��ָ�룩
    std::shared_ptr<Material> material;     //�������

    // TODO:��Χ��
};
