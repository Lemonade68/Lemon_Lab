#pragma once
#include "Geometry.h"
#include "FunctionLayer/Ray/Ray.h"
#include "FunctionLayer/Acceleration/AABB.h"

struct Transform{
public:
    //������ά�ռ������ֲ����ľ�������(����������Ĳ������������Ĳ���)
    static Matrix4f translation(const Vector3f &offset);
    static Matrix4f rotation(const Vector3f &axis, float radian);
    static Matrix4f scalation(const Vector3f &scale);

    Transform();        //Ĭ�Ϲ��캯��������4*4�ĵ�λ����
    Transform(const Matrix4f &_translation, const Matrix4f &_rotation,
              const Matrix4f &_scalation);

    //�ֲ��ռ�����������ռ�����
    Vector3f toWorld(const Vector3f &vector) const;   
    Point3f toWorld(const Point3f &point) const;

    //TODO���԰�Χ�н��б仯
    //* �԰�Χ�н��б任
    AABB toWorld(const AABB & box) const;

    //���ؽ�����ռ��µ�rayת��Ϊ����ռ��ray�����ı�ԭ����ray��
    Ray RayToLocal(const Ray &ray) const;

public:
    Matrix4f translate, invTranslate;
    Matrix4f rotate, invRotate;
    Matrix4f scale, invScale;
};


//����3ά�ռ��пɽ�����ת�����š�ƽ�ƵĶ�����Ҫ�̳и���
class Transformable{
public:
    Transformable() = default;
    Transformable(const Transform &trans) : transform(trans) {}

// protected:
    Transform transform;    //��¼������ռ䵽����ռ�ı仯(inv����任)
};