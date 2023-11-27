#pragma once
#include"Shape.h"
// #include"FunctionLayer/Material/Matte.h"     //Shape.h中已经包含
#include"FunctionLayer/Light/AreaLight.h"

class Sphere : public Shape{
public:
    // Sphere() = delete;
    
    //传入的是球体局部坐标系中的坐标        --默认使用matte材质，不是光源
    Sphere(Point3f o = Point3f(.0f, .0f, .0f), float r = 1.f,
           const std::shared_ptr<Material> &_material = std::make_shared<Matte_Material>(),
           const std::shared_ptr<Light> &_light = nullptr,
           const Vector3f &translate = Vector3f(.0f, .0f, .0f),
           const Vector3f &scale = Vector3f(1.f, 1.f, 1.f)); // 球体暂时不考虑旋转与缩放（缩放直接更改半径）

    virtual bool rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const override;

    virtual void fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const override;

    //计算重心坐标工具函数（其实就是phi和theta）
    void Sphere::get_sphere_uv(const Vector3f &normal, float *u, float *v) const;

    virtual void debugPrint() const override{
        std::cout << "<Sphere>\ncenter: ";
        center.debugPrint();
        std::cout << "radius: " << radius << std::endl;
    }


    //暂时不使用球类作为面光源，后续完善
    virtual void uniformSampleOnSurface(Vector2f sample,
                                      Intersection &intersection,
                                      float &pdf) const override {
        // TODO finish this
        return;
    }
    
public:
    Point3f center;
    float radius;
};

