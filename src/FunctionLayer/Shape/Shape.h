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
    //默认构造函数：不平移，不缩放，不旋转
    Shape(const Vector3f &translate = Vector3f(.0f, .0f, .0f),
          const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
          const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
          float radian = .0f);

    //TODO：使用json初始化shape

    //光线与物体求交函数
    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const = 0;

public:
    std::shared_ptr<Light> light;           //物体自身是否发光（多态，使用指针）
    std::shared_ptr<Material> material;     //物体材质

    // TODO:包围盒
};
