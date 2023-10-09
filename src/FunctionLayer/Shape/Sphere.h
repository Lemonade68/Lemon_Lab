#pragma once
#include"Shape.h"

class Sphere : public Shape{
public:
    // Sphere() = delete;
    
    //�����������ֲ�����ϵ�е�����
    Sphere(Point3f o = Point3f(.0f, .0f, .0f), float r = 1.f,
           const Vector3f &translate = Vector3f(.0f, .0f, .0f),
           const Vector3f &scale = Vector3f(1.f, 1.f, 1.f));        //������ʱ��������ת

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    //��ȡp��λ�ô�����������
    //���⣺ʵ��ʱĬ���Ǿֲ����꣬����sphere��ʵ���������������µ�      ����    ���뷨�߿��Խ����
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

