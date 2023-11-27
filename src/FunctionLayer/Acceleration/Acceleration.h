#pragma once

#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Ray/Ray.h"
#include<vector>


class Acceleration{
public:
    Acceleration() = default;

    Acceleration(const std::vector<std::shared_ptr<Shape>> &_shapeList)
        : shapeList(_shapeList) {}

    virtual void build() = 0;       //����������shapeList���������ٽṹ

    //����Acceleration����ӿ�
    std::optional<Intersection> rayIntersect(Ray &ray){
        int primID = 0, geomID = -1;
        float u, v;
        bool hit = rayIntersect(ray, &geomID, &primID, &u, &v);
        if(!hit)
            return std::nullopt;
        Intersection its;
        shapeList[geomID]->fillIntersection(ray.tFar, primID, u, v, &its);      //��Ӧ����embree��RTCHit��geomID,primID,u,v,tFar
        return std::make_optional(its);
    }

    //�ڲ����ܺ�����ʵ����ʹ�ã�
    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const = 0;    //������󽻵Ľӿ�

    void attchShape(const std::shared_ptr<Shape> &shape){
        shapeList.push_back(shape);
    }

    virtual void debugPrint() const = 0;

    AABB boundingBox;       //�������ٽṹ��bounding box
    
    std::vector<std::shared_ptr<Shape>> shapeList;      //���ٽṹ���������б�
};