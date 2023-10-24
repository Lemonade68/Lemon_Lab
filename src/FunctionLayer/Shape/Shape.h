#pragma once

#include"Intersection.h"
#include"CoreLayer/Math/Transform.h"
#include"FunctionLayer/Ray/Ray.h"

#include"FunctionLayer/Material/Matte.h"
#include"FunctionLayer/Light/Light.h"

#include"FunctionLayer/Acceleration/AABB.h"

#include<optional>

//如果这边在头文件中使用include来包含对应的头文件时，会形成头文件闭环，从而不能pragma 第二次，造成头文件无法展开
//从而出现识别不出来的问题（未定义基类等等）
// class Light;
// class Material;
// extern class Matte_Material;

class Shape : public Transformable{
public:
    //默认构造函数：不平移，不缩放，不旋转
    Shape(
        const std::shared_ptr<Material> &_material = std::make_shared<Matte_Material>(),
        const std::shared_ptr<Light> &_light = nullptr,
        const Vector3f &translate = Vector3f(.0f, .0f, .0f),
        const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
        const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
        float radian = .0f);

    //TODO：使用json初始化shape

    //光线与物体求交函数
    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const = 0;

    //针对面光源使用：在物体表面进行均匀随机采样，采样点的信息直接存在intersection中（方便）
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const = 0;

    virtual void debugPrint() const = 0;    //打印物体信息

    AABB getAABB() const { return boundingBox; }

public:
    std::shared_ptr<Light> light;           //物体自身是否发光（多态，使用指针）
    std::shared_ptr<Material> material;     //物体材质

    // TODO:包围盒
    AABB boundingBox;
};
