#pragma once
#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class Shape;

// ray��shape���㴦����Ϣ
struct Intersection {
    // float distance;         //������㵽����ľ���
    float t;                //���㴦t��ֵ����ʵ���������distance��
    Point3f position;       //����λ��
    Vector3f normal;        //���㷨��(��λ��)
    const Shape* shape;     //������ཻ������

    //TODO �������꣬�����븱���ߣ�����΢��
    Vector2f texCoord;      //��������

    //�����������븱���ߣ���BSDF�ľֲ�����ϵʹ��
    //�����߾�����ʲô������ʵ����ν��ֻ��Ҫ�ܹ�����һ���ֲ�����ϵ���ɣ�ע�⴫�뵥λ����
    Vector3f tangent, bitangent;

    //Ϊ�˶���Embreeʹ��            --��Ϊ��rayIntersectShape���зֿ�����
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
