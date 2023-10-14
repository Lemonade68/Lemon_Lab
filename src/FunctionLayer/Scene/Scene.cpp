#include"Scene.h"
#include"CoreLayer/Math/Function.h"

std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    bool hit = false, result = false;
    Intersection intersection;
    for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
        result = (*it)->rayIntersectShape(ray, intersection);       //场景中物体与光线求交
        if(result)
            hit = result;
    }
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
    
    int i = (int)(sample / p);          //转换为整数，范围应该在[0, light_num]
    Clamp(i, 0, light_num - 1);         //i的范围限制在[0,light_num - 1]中
    //返回第i个光源                     //出问题的地方在这里(i是int类型，没有显式初始化，会调用随机值)
    return light_list[i];
}