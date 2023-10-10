#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Intersection.h"

#include<optional>
#include<vector>
#include<memory>


//暂时未考虑加速结构
class Scene {
public:
    Scene() = default;  

    Scene(std::vector<std::shared_ptr<Shape>> _shape_list) : shape_list(_shape_list) {}

    //添加物体
    void add(const std::shared_ptr<Shape> &ptr){
        shape_list.push_back(ptr);
    }

    //光线与场景求交
    std::optional<Intersection> rayIntersect(Ray &ray) const;

private:
    std::vector<std::shared_ptr<Shape>> shape_list;

    //TODO：考虑acceleration
};