#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class AABB{
public:
    AABB(){
        //��ʼ��ʱ����Сֵ����Ϊ������ֵ����Ϊ��С���κθı䶼����ֱ�Ӹı��Χ��
        pMin = Point3f(FLT_MAX, FLT_MAX, FLT_MAX);
        pMax = Point3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }

    AABB(const Point3f &_pMin, const Point3f &_pMax) : pMin(_pMin), pMax(_pMax) {}

    //�ϲ�������Χ�У�������һ���µ�AABB
    AABB Union(const AABB &other) const;

    //��ԭ����AABB�����ţ��ں���һ��AABB��
    void Expand(const AABB &other);

    //ͨ��һ�����������µ�AABB��ʹ������õ�
    AABB Union(const Point3f &other) const;

    //��ԭ����AABB�����ţ�������һ���㣩
    void Expand(const Point3f &other);

    //�жϵ�ǰ��Χ���Ƿ����other��Χ��
    bool Overlap(const AABB &other) const;

    //�жϹ����Ƿ����Χ���н��㣬���ؽ�����뿪��Χ�е�ʱ��
    bool RayIntersect(Ray &ray, float *tMin = nullptr, float *tMax = nullptr) const;

    //���ذ�Χ�����ģ���Χ�������㷨��ʹ�ã�
    Point3f Center() const;

    void debugPrint() const{
        std::cout << "Bounding box  ����  pMin: ";
        pMin.debugPrint();
        std::cout << "pMax: ";
        pMax.debugPrint();
    }

public:
    Point3f pMin, pMax;
};