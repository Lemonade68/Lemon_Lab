#include"Camera.h"

PerspectiveCamera::PerspectiveCamera(float _aspectRatio, float _verticalFOV, 
        const Point3f &_position, const Point3f &_lookAt, const Vector3f &_up) : Camera(){
    position = _position;
    lookAt = _lookAt;
    up = _up;
    aspectRatio = _aspectRatio;
    verticalFOV = _verticalFOV / 180.f * PI;      //ת��Ϊ������

    Vector3f forward = normalize(lookAt - position),
             right = normalize(cross(forward, up));
    up = normalize(cross(right, forward));      //���¸���up���������ָ��������ռ���Ϸ���

    //���������transform��������(ֻ��translate��rotate)
    Matrix4f translation = Transform::translation(p2v(position));   //����ֱ����position���ǵ�
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

    rotation = transpose(rotation);     //������

    transform = Transform(translation, rotation, Matrix4f(1.f));    //������
}