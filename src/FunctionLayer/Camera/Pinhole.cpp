#include"Pinhole.h"

PinholeCamera::PinholeCamera(float _aspectRatio, float _verticalFOV,
                             const Point3f &_position, const Point3f &_lookAt, const Vector3f &_up)
    : PerspectiveCamera(_aspectRatio, _verticalFOV, _position, _lookAt, _up){}


Ray PinholeCamera::sampleRay(const CameraSample &sample, const Vector2f &NDC) const{
    //-------------����ռ���------------
    //��ͼ����NDC��xy���궼ת��Ϊ(-0.5, 0.5)�ķ�Χ������������Ļ���ģ�ע��������0.5�ļ���˳�򣬺���϶�Ӧ�ֱ���
    float x = (NDC[0] - 0.5f) * 1200.f + sample.xy[0];         //����1200�����()
    float y = (0.5f - NDC[1]) * 800.f + sample.xy[1];

    float tanHalfFov = std::tan(verticalFOV * 0.5f);
    float z = -800.f * 0.5f / tanHalfFov;       //�������ƽ��ľ��룬ע�⸺��(���ߵ�����ϵ��z������-forward�����)

    Vector3f direction = Vector3f(x, y, z);     //�������ϵ�µĹ��߷���

    //----------�任������ռ���----------

    direction = normalize(transform.toWorld(direction));
    Point3f origin = transform.toWorld(Point3f(.0f));       //�����������������λ��

    return Ray(origin, direction, tNear, tFar, startTime);
}

//TODO��΢�ֹ��߲���

