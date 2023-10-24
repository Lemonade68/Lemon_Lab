#pragma once
#include "Geometry.h"
#include "FunctionLayer/Ray/Ray.h"
#include "FunctionLayer/Acceleration/AABB.h"

struct Transform{
public:
    //返回三维空间中三种操作的矩阵描述(属于类自身的操作，非类对象的操作)
    static Matrix4f translation(const Vector3f &offset);
    static Matrix4f rotation(const Vector3f &axis, float radian);
    static Matrix4f scalation(const Vector3f &scale);

    Transform();        //默认构造函数，创建4*4的单位矩阵
    Transform(const Matrix4f &_translation, const Matrix4f &_rotation,
              const Matrix4f &_scalation);

    //局部空间坐标变回世界空间坐标
    Vector3f toWorld(const Vector3f &vector) const;   
    Point3f toWorld(const Point3f &point) const;

    //TODO：对包围盒进行变化
    //* 对包围盒进行变换
    AABB toWorld(const AABB & box) const;

    //返回将世界空间下的ray转化为物体空间的ray（不改变原来的ray）
    Ray RayToLocal(const Ray &ray) const;

public:
    Matrix4f translate, invTranslate;
    Matrix4f rotate, invRotate;
    Matrix4f scale, invScale;
};


//所有3维空间中可进行旋转、缩放、平移的对象都需要继承该类
class Transformable{
public:
    Transformable() = default;
    Transformable(const Transform &trans) : transform(trans) {}

// protected:
    Transform transform;    //记录从物体空间到世界空间的变化(inv是逆变换)
};