#pragma once

#include"FunctionLayer/Ray/Ray.h"
#include"CoreLayer/Math/Transform.h"
#include"CoreLayer/Math/Constant.h"

//�����Ҫ�������
struct CameraSample{
    Vector2f xy;    //�������ϲ����������
    Vector2f lens;  //��͸���ϲ����������
    float time;     //���Ŵ򿪵�ʱ����ڲ���
};

class Camera : public Transformable{
public:
    // Camera() = delete;

    //����transform�����ض��Ķ���Ĺ��캯�����������
    Camera(float t_n = EPSILON, float t_f = 1e10f, float st = .0f, float et = .0f)
        : tNear(t_n), tFar(t_f), startTime(st), endTime(et) {}

    //*   ����һ���������sample�����������һ������ռ��µĹ���
    //*   NDC��normalized device coordinate����ΧΪ[0,0]-[1,1]����Ӧ���£�
    //*   [0, 0]----X----
    //*     |           |
    //*     Y           |
    //*     ----------[1, 1]
    virtual Ray sampleRay(const CameraSample &sample, const Vector2f &NDC) const = 0;

    //TODO������΢�ֹ��ߵķ���
protected:
    float tNear, tFar, startTime, endTime;  //���������Զ���룬���ſ����ر�ʱ��
};


//͸�����
//��ָ�����λ�á�ָ��ĵ㡢���Ϸ���
class PerspectiveCamera : public Camera{
public:
    // PerspectiveCamera() = delete;

    PerspectiveCamera(float _aspectRatio, float _verticalFOV = 80.f,
                      const Point3f &_position = Point3f(.0f, 2.f, 5.f),
                      const Point3f &_lookAt = Point3f(.0f, .0f, .0f),
                      const Vector3f &_up = Vector3f(.0f, 1.f, .0f));

    virtual Ray sampleRay(const CameraSample &sample, const Vector2f &NDC) const = 0;   //��Ȼ�ǳ�����(��Ϊpinhole��lens)
    
    //TODO������΢�ֹ��ߵķ���

protected:
    Point3f position, lookAt;
    Vector3f up;
    float verticalFOV, aspectRatio;     //��ֱFOV����Ļ�����
};

