#include"Shape.h"

//初始化物体时需要将其transform初始化
Shape::Shape(const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian){
    //获取矩阵描述
    Matrix4f tranlsateMat = Transform::translation(translate);
    Matrix4f scaleMat = Transform::scalation(scale);
    Matrix4f rotateMat = Transform::rotation(axis, radian);

    transform = Transform(tranlsateMat, rotateMat, scaleMat);
}

//TODO:后续东西添加
