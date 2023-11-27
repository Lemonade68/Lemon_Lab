#include"Scene.h"
#include"CoreLayer/Math/Function.h"
#include"FunctionLayer/Acceleration/BVH.h"
#include"FunctionLayer/Acceleration/EmbreeBVH.h"

//有了包围盒后新增：
void Scene::buildSceneAcceleration(){
    // acceleration = std::make_shared<BVH>(shape_list, 1);   //获取物体列表
    acceleration = std::make_shared<EmbreeBVH>(shape_list);   //获取物体列表
    acceleration->build();      //构建BVH加速结构
}

//有了加速结构后的写法
std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    return acceleration->rayIntersect(ray);
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