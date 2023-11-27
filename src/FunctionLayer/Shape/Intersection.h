#pragma once
#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class Shape;

// ray与shape交点处的信息
struct Intersection {
    // float distance;         //光线起点到交点的距离
    float t;                //交点处t的值（其实就是上面的distance）
    Point3f position;       //交点位置
    Vector3f normal;        //交点法线(单位化)
    const Shape* shape;     //与光线相交的物体

    //TODO 纹理坐标，切线与副切线，光线微分
    Vector2f texCoord;      //纹理坐标

    //新增：切线与副切线：给BSDF的局部坐标系使用
    //这两者具体是什么方向其实无所谓，只需要能构建出一个局部坐标系即可，注意传入单位化的
    Vector3f tangent, bitangent;

    //为了对于Embree使用            --改为对rayIntersectShape进行分开计算
    // int primID, geomID;  

public:
    void debugPrint(){
        std::cout << "t = " << t << std::endl;
        std::cout << "position:\n";
        position.debugPrint();
        std::cout << "normal:\n";
        normal.debugPrint();
    }
};
