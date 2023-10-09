#pragma once
#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class Shape;

// ray��shape���㴦����Ϣ
struct Intersection {
    // float distance;         //������㵽����ľ���
    double t;               //���㴦t��ֵ
    Point3f position;       //����λ��
    Vector3f normal;        //���㷨��
    const Shape* shape;     //������ཻ������

    //TODO �������꣬�����븱���ߣ�����΢��
    Vector2f texCoord;      //��������

public:
    void debugPrint(){
        std::cout << "t = " << t << std::endl;
        std::cout << "position:\n";
        position.debugPrint();
        std::cout << "normal:\n";
        normal.debugPrint();
    }
};
