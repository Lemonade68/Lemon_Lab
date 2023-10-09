#pragma once
#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class Shape;

// ray与shape交点处的信息
struct Intersection {
    // float distance;         //光线起点到交点的距离
    double t;               //交点处t的值
    Point3f position;       //交点位置
    Vector3f normal;        //交点法线
    const Shape* shape;     //与光线相交的物体

    //TODO 纹理坐标，切线与副切线，光线微分
    Vector2f texCoord;      //纹理坐标

public:
    void debugPrint(){
        std::cout << "t = " << t << std::endl;
        std::cout << "position:\n";
        position.debugPrint();
        std::cout << "normal:\n";
        normal.debugPrint();
    }
};
