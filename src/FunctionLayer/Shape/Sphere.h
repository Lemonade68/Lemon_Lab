#pragma once
#include"Shape.h"
#include"FunctionLayer/Material/Matte.h"
#include"FunctionLayer/Light/AreaLight.h"

class Sphere : public Shape{
public:
    // Sphere() = delete;
    
    //�����������ֲ�����ϵ�е�����        --Ĭ��ʹ��matte���ʣ����ǹ�Դ
    Sphere(Point3f o = Point3f(.0f, .0f, .0f), float r = 1.f,
           const std::shared_ptr<Material> &_material = std::make_shared<Matte_Material>(),
           const std::shared_ptr<Light> &_light = nullptr,
           const Vector3f &translate = Vector3f(.0f, .0f, .0f),
           const Vector3f &scale = Vector3f(1.f, 1.f, 1.f)); // ������ʱ��������ת

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    //��ȡp��λ�ô�����������
    //���⣺ʵ��ʱĬ���Ǿֲ����꣬����sphere��ʵ���������������µ�      ����    ���뷨�߿��Խ����
    void get_sphere_uv(const Vector3f &normal, Vector2f &texCoord) const;

    virtual void debugPrint() const override{
        std::cout << "<Sphere>\ncenter: ";
        center.debugPrint();
        std::cout << "radius: " << radius << std::endl;
    }


    //��ʱ��ʹ��������Ϊ���Դ����������
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

