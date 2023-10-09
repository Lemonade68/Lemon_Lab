#include"Shape.h"

//��ʼ������ʱ��Ҫ����transform��ʼ��
Shape::Shape(const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian){
    //��ȡ��������
    Matrix4f tranlsateMat = Transform::translation(translate);
    Matrix4f scaleMat = Transform::scalation(scale);
    Matrix4f rotateMat = Transform::rotation(axis, radian);

    transform = Transform(tranlsateMat, rotateMat, scaleMat);
}

//TODO:�����������
