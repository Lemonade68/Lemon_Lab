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

    virtual bool rayIntersect(Ray &ray, Intersection &intersection) const = 0;    //������󽻵Ľӿ�

    void attchShape(const std::shared_ptr<Shape> &shape){
        shapeList.push_back(shape);
    }

    virtual void debugPrint() const = 0;

protected:
    std::vector<std::shared_ptr<Shape>> shapeList;      //���ٽṹ���������б�
};