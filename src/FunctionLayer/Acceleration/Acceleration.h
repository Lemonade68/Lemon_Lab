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

    virtual bool rayIntersect(Ray &ray, Intersection &intersection) const = 0;    //与光线求交的接口

    void attchShape(const std::shared_ptr<Shape> &shape){
        shapeList.push_back(shape);
    }

    virtual void debugPrint() const = 0;

protected:
    std::vector<std::shared_ptr<Shape>> shapeList;      //加速结构包含物体列表
};