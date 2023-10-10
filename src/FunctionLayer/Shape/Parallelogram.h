#pragma once

#include"Shape.h"

//ƽ���ı���
class Parallelogram : public Shape{
public:
    Parallelogram() = delete;       //����ָ��һ��ƽ���ı���

    Parallelogram(const Point3f &p, const Vector3f &e1, const Vector3f &e2,
                  const Vector3f &translate = Vector3f(.0f, .0f, .0f),
                  const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
                  const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
                  float radian = .0f);

    virtual bool rayIntersectShape(Ray &ray, Intersection &interscetion) const override;

    //TODO�����ɹ�Դʱ�Ĳ�������


public:
    Point3f base;               //�����ߵĹ�ͬ���
    Vector3f edge0, edge1;      //�����ߵ�����(ע������ϵ���������Ϊ�Ϸ��߷���)
};