#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Intersection.h"
#include"FunctionLayer/Light/AreaLight.h"

#include"FunctionLayer/Acceleration/Acceleration.h"      //暂时默认使用BVH加速

#include<optional>
#include<vector>
#include<memory>


//暂时未考虑加速结构(现已考虑)
class Scene {
public:
    Scene() { 
        light_num = 0;
        acceleration = nullptr;
    }

    Scene(std::vector<std::shared_ptr<Shape>> _shape_list, std::vector<std::shared_ptr<Light>> _light_list)
        : shape_list(_shape_list), light_list(_light_list), light_num(_light_list.size()) {}

    //添加物体
    void addObject(const std::shared_ptr<Shape> &ptr){
        shape_list.push_back(ptr);
    }

    //添加光源（面光源的话将其shape添加自发光项后也添加到shape_list中）
    void addLight(const std::shared_ptr<Light> &light){
        if(light->type ==LightType::EnvironmentLight)       //环境光源（后续更改）
            return;
        //面光源：也添加到物体列表中（因为有体积，也可以在画面中显示）
        if(light->type == LightType::AreaLight){
            //学习下这种写法（基类没有但是要添加子类成员）
            auto shape = std::static_pointer_cast<AreaLight>(light)->shape;
            shape->light = light;
            shape_list.push_back(shape);
        }
        
        light_list.push_back(light);
        ++light_num;
    }

    //建立场景的加速结构
    void buildSceneAcceleration();

    //光线与场景求交（添加加速结构后发生变化）
    std::optional<Intersection> rayIntersect(Ray &ray) const;

    //TODO：添加环境光
    // std::vector<std::shared_ptr<InfiniteLight>> InfiniteLights;

    //从场景中采样一个光源，返回该光源的指针，并返回采样到该光源的概率（默认为均匀采样）
    std::shared_ptr<Light> sampleLight(float sample, float &pdf) const;


    void debugPrintLight(){
        std::cout << "Light Information:==============================\n";
        std::cout << "light number: " << light_num << std::endl;
        for (auto it = light_list.begin(); it != light_list.end(); ++it){
            (*it)->debugPrint();
        }
        std::cout << "================================================\n";
    }

    //打印物体信息，如果发光则也打印发光信息
    void debugPrintObject(){
        std::cout << "Object Information:==============================\n";
        for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
            (*it)->debugPrint();
            if((*it)->light){
                printf("- Has light propertity, parameter:\n");
                (*it)->light->debugPrint();
            }
        }
        std::cout << "=================================================\n";
    }

    void debugPrintAccleration(){
        acceleration->debugPrint();
    }

private:
    std::vector<std::shared_ptr<Shape>> shape_list;
    std::vector<std::shared_ptr<Light>> light_list;     //光源列表

    std::shared_ptr<Acceleration> acceleration;         //加速结构

    int light_num;              //内置类型要是不显式初始化的话会产生随机值

    //TODO：考虑acceleration
};