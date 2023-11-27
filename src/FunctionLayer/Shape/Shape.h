#pragma once

#include"Intersection.h"
#include"CoreLayer/Math/Transform.h"
#include"FunctionLayer/Ray/Ray.h"

#include"FunctionLayer/Material/Matte.h"
#include"FunctionLayer/Light/Light.h"

#include"FunctionLayer/Acceleration/AABB.h"
#include"embree3/rtcore.h"

#include<optional>

//��������ͷ�ļ���ʹ��include��������Ӧ��ͷ�ļ�ʱ�����γ�ͷ�ļ��ջ����Ӷ�����pragma �ڶ��Σ����ͷ�ļ��޷�չ��
//�Ӷ�����ʶ�𲻳��������⣨δ�������ȵȣ�
// class Light;
// class Material;
// extern class Matte_Material;

//�û������rtcRayHit�ࣨ�����һ��intersectionָ�룬���ڴ����ཻ���ݣ�
class UserRayHit1 : public RTCRayHit{
public:
    std::shared_ptr<Intersection> its = nullptr;
};

class Shape : public Transformable{
public:
    //Ĭ�Ϲ��캯������ƽ�ƣ������ţ�����ת
    Shape(
        const std::shared_ptr<Material> &_material = std::make_shared<Matte_Material>(),
        const std::shared_ptr<Light> &_light = nullptr,
        const Vector3f &translate = Vector3f(.0f, .0f, .0f),
        const Vector3f &scale = Vector3f(1.f, 1.f, 1.f),
        const Vector3f &axis = Vector3f(.0f, 1.f, 0.f),
        float radian = .0f);

    //TODO��ʹ��json��ʼ��shape

    //�����������󽻺�����u/vΪ��������(embreeʹ��)��primIDΪ�����ϵ���Ԫid
    virtual bool rayIntersectShape(Ray &ray, int* primID, float *u, float *v) const = 0;

    //���û�ȡ����Ϣ������ʵ�ʵ�intersection����Ϊembree�ڲ��󽻺󷵻���Щ��Ϣ����˽��䵥����Ϊһ��������
    virtual void fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const = 0;

    //������Դʹ�ã������������о���������������������Ϣֱ�Ӵ���intersection�У����㣩
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const = 0;

    virtual void debugPrint() const = 0;    //��ӡ������Ϣ

    AABB getAABB() const { return boundingBox; }

    //��ȡһ�������embree������������mesh���¶��壩
    virtual RTCGeometry getEmbreeGeometry(RTCDevice device) const;

public:
    int geometryID;         //���������id��triangle mesh�����������ζ�Ӧһ��id��
    std::shared_ptr<Light> light;           //���������Ƿ񷢹⣨��̬��ʹ��ָ�룩
    std::shared_ptr<Material> material;     //�������

    // TODO:��Χ��
    AABB boundingBox;
};
