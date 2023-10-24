#include"Scene.h"
#include"CoreLayer/Math/Function.h"
#include"FunctionLayer/Acceleration/BVH.h"

//有了包围盒后新增：
void Scene::buildSceneAcceleration(){
    acceleration = std::make_shared<BVH>(shape_list);   //获取物体列表
    acceleration->build();      //构建BVH加速结构
}

std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    //这里是没有加速结构时的写法
    // bool hit = false, result = false;
    // Intersection intersection;
    // for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
    //     result = (*it)->rayIntersectShape(ray, intersection);       //场景中物体与光线求交
    //     if(result)
    //         hit = result;
    // }
    // if(!hit)
    //     return std::nullopt;
    // return std::make_optional(intersection);


    //下面是有了加速结构的写法
    Intersection intersection;
    bool hit = acceleration->rayIntersect(ray, intersection);

    if(!hit)
        return std::nullopt;
    return std::make_optional(intersection);
}

//这里出现过segmentation fault
std::shared_ptr<Light> Scene::sampleLight(float sample, float &pdf) const{
    if(light_num == 0)                  //没有光源：直接返回
        return nullptr;

    float p = 1.f / light_num;
    pdf = p;                            //均匀采样光源——pdf直接返回p

    int i = static_cast<int>(sample / p);   // 转换为整数，范围应该在[0, light_num]
    
    // 该函数需要使用返回值来更改数值，传入的不是引用！
    i = Clamp(i, 0, light_num - 1);             //i的范围限制在[0,light_num - 1]中
    
    //返回第i个光源                       //出问题的地方在这里(i是int类型，没有显式初始化，会调用随机值)
    return light_list[i];
}