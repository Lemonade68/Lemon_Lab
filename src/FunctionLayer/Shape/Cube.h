#pragma once

#include"Shape.h"
#include"FunctionLayer/Light/Light.h"

class Cube : public Shape{
public:
    Cube(const Point3f &_boxMin = Point3f(-1.f, -1.f, -1.f), const Point3f &_boxMax = Point3f(1.f, 1.f, 1.f),
         const std::shared_ptr<Material> _material = std::make_shared<Matte_Material>(),
         const std::shared_ptr<Light> _light= nullptr,
         const Vector3f &_translate = Vector3f(.0f),
         const Vector3f &_scale = Vector3f(1.f),
         const Vector3f &_axis = Vector3f(.0f, 1.f, .0f),
         float radian = .0f);

    //TODO��ʹ��JSON�ļ�����cube��Ϣ

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    void fillIntersection(float t, int faceID, float u, float v, Intersection &intersection) const;

    virtual void debugPrint() const override{
        std::cout << "<Cube>\nboxMin: ";
        boxMin.debugPrint();
        std::cout << "boxMax: ";
        boxMax.debugPrint();
    }

    //��ʱ��ʹ����������Ϊ��Դ����������
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override{
        //TODO��finish this
        return;
    }

private:
    Point3f boxMin, boxMax;     //�������������µ������ǰ�ϵ㣨��Ӧ��������ֵ����Сֵ��
};