#pragma once

#include"CoreLayer/Math/Math.h"

struct Ray{
    Ray() = default;
    
    //起点和方向的构造函数
    Ray(Point3f _origin, Vector3f _direction, float _tNear = EPSILON, 
        float _tFar = FLT_MAX, float _time = .0f)
        : origin(_origin), direction(normalize(_direction)), tNear(_tNear),
          tFar(_tFar), time(_time) {}
    
    //起点和终点的构造函数
    Ray(Point3f _origin, Point3f _destination, float _time  = .0f)
        : origin(_origin), tNear(EPSILON), time(_time){
        Vector3f o2d = _destination - _origin;
        tFar = o2d.length() - EPSILON;
        direction = normalize(o2d);
    }

    //返回光线上某点的位置
    inline Point3f at(float distance) const{
        return origin + distance * direction;
    }

    void debugPrint() const{
        std::cout << "<Ray>  origin:\n";
        origin.debugPrint();
        std::cout << "direction:\n";
        direction.debugPrint();
        std::cout << "tFar: " << tFar << "; "
                  << "tNear: " << tNear << "\n";
    }

public:
    //基本参数
    Point3f origin;
    Vector3f direction;     
    float tFar, tNear;      //限制光线范围的t
    float time;             //光线对应的时间（动态模糊需要）

    //光线微分（待做）
    //***
};
