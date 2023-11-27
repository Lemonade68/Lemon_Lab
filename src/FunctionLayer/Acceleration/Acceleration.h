#pragma once

#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Ray/Ray.h"
#include<vector>


class Acceleration{
public:
    Acceleration() = default;

    Acceleration(const std::vector<std::shared_ptr<Shape>> &_shapeList)
        : shapeList(_shapeList) {}

    virtual void build() = 0;       //根据完整的shapeList来构建加速结构

    //所有Acceleration对外接口
    std::optional<Intersection> rayIntersect(Ray &ray){
        int primID = 0, geomID = -1;
        float u, v;
        bool hit = rayIntersect(ray, &geomID, &primID, &u, &v);
        if(!hit)
            return std::nullopt;
        Intersection its;
        shapeList[geomID]->fillIntersection(ray.tFar, primID, u, v, &its);      //对应的是embree中RTCHit的geomID,primID,u,v,tFar
        return std::make_optional(its);
    }

    //内部功能函数（实际求交使用）
    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const = 0;    //与光线求交的接口

    void attchShape(const std::shared_ptr<Shape> &shape){
        shapeList.push_back(shape);
    }

    virtual void debugPrint() const = 0;

    AABB boundingBox;       //整个加速结构的bounding box
    
    std::vector<std::shared_ptr<Shape>> shapeList;      //加速结构包含物体列表
};