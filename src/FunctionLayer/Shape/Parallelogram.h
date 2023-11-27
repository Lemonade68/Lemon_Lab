#pragma once

#include"Shape.h"

//平行四边形
class Parallelogram : public Shape{
public:
    Parallelogram() = delete;       //必须指定一个平行四边形(交点和两条边)

    Parallelogram(const Point3f &p, const Vector3f &e1, const Vector3f &e2,
                  const std::shared_ptr<Light> &_light = nullptr,
                  const std::shared_ptr<Material> &_material = std::make_shared<Matte_Material>(),
                  const Vector3f &translate = Vector3f(.0f, .0f, .0f),
                  const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
                  const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
                  float radian = .0f);

    virtual bool rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const override;

    virtual void fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const override;

    //TODO：当成光源时的采样函数
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &intersection, float &pdf) const override;

    virtual void debugPrint() const override{
        std::cout << "<Parallelogram>\nbase: ";
        base.debugPrint();
        std::cout << "edge0: ";
        edge0.debugPrint();
        std::cout << "edge1: ";
        edge1.debugPrint();
    }

public:
    Point3f base;               //两条边的共同起点
    Vector3f edge0, edge1;      //两条边的向量(注意右手系，叉积方向为上法线方向)
};