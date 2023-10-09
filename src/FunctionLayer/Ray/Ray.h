#pragma once

#include"CoreLayer/Math/Math.h"

struct Ray{
    Ray() = default;
    
    //���ͷ���Ĺ��캯��
    Ray(Point3f _origin, Vector3f _direction, float _tNear = EPSILON, 
        float _tFar = FLT_MAX, float _time = .0f)
        : origin(_origin), direction(normalize(_direction)), tNear(_tNear),
          tFar(_tFar), time(_time) {}
    
    //�����յ�Ĺ��캯��
    Ray(Point3f _origin, Point3f _destination, float _time  = .0f)
        : origin(_origin), tNear(EPSILON), time(_time){
        Vector3f o2d = _destination - _origin;
        tFar = o2d.length() - EPSILON;
        direction = normalize(o2d);
    }

    //���ع�����ĳ���λ��
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
    //��������
    Point3f origin;
    Vector3f direction;     
    float tFar, tNear;      //���ƹ��߷�Χ��t
    float time;             //���߶�Ӧ��ʱ�䣨��̬ģ����Ҫ��

    //����΢�֣�������
    //***
};
