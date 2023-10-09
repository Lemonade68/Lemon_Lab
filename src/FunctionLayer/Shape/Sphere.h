#pragma once
#include"Shape.h"

class Sphere : public Shape{
public:
    // Sphere() = delete;
    
    //传入的是球体局部坐标系中的坐标
    Sphere(Point3f o = Point3f(.0f, .0f, .0f), float r = 1.f,
           const Vector3f &translate = Vector3f(.0f, .0f, .0f),
           const Vector3f &scale = Vector3f(1.f, 1.f, 1.f));        //球体暂时不考虑旋转

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    //获取p点位置处的纹理坐标
    //问题：实现时默认是局部坐标，这里sphere的实现是在世界坐标下的      ——    传入法线可以解决？
    void get_sphere_uv(const Vector3f &normal, Vector2f &texCoord) const;

    void debugPrint(){
        std::cout << "center:\n";
        center.debugPrint();
        std::cout << "radius: " << radius << std::endl;
    }
    
public:
    Point3f center;
    float radius;
};

