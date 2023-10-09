#include"Camera.h"

PerspectiveCamera::PerspectiveCamera(float _aspectRatio, float _verticalFOV, 
        const Point3f &_position, const Point3f &_lookAt, const Vector3f &_up) : Camera(){
    position = _position;
    lookAt = _lookAt;
    up = _up;
    aspectRatio = _aspectRatio;
    verticalFOV = _verticalFOV / 180.f * PI;      //转换为弧度制

    Vector3f forward = normalize(lookAt - position),
             right = normalize(cross(forward, up));
    up = normalize(cross(right, forward));      //重新更新up向量（这次指向摄像机空间的上方）

    //配置相机的transform对象属性(只搞translate和rotate)
    Matrix4f translation = Transform::translation(p2v(position));   //不能直接用position，是点
    Matrix4f rotation;

    rotation[0][0] = right[0];
    rotation[1][0] = right[1];
    rotation[2][0] = right[2];
    rotation[3][0] = 0;

    rotation[0][1] = up[0];
    rotation[1][1] = up[1];
    rotation[2][1] = up[2];
    rotation[3][1] = 0;

    rotation[0][2] = -forward[0];
    rotation[1][2] = -forward[1];
    rotation[2][2] = -forward[2];
    rotation[3][2] = 0;

    rotation[0][3] = 0;
    rotation[1][3] = 0;
    rotation[2][3] = 0;
    rotation[3][3] = 1;

    rotation = transpose(rotation);     //列主序

    transform = Transform(translation, rotation, Matrix4f(1.f));    //不缩放
}