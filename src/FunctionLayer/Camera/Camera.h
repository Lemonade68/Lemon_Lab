#pragma once

#include"FunctionLayer/Ray/Ray.h"
#include"CoreLayer/Math/Transform.h"
#include"CoreLayer/Math/Constant.h"

//相机需要的随机数
struct CameraSample{
    Vector2f xy;    //在像素上采样的随机数
    Vector2f lens;  //在透镜上采样的随机数
    float time;     //快门打开的时间段内采样
};

class Camera : public Transformable{
public:
    // Camera() = delete;

    //对于transform：到特定的对象的构造函数那里再添加
    Camera(float t_n = EPSILON, float t_f = 1e10f, float st = .0f, float et = .0f)
        : tNear(t_n), tFar(t_f), startTime(st), endTime(et) {}

    //*   接受一个随机采样sample，从相机产生一条世界空间下的光线
    //*   NDC：normalized device coordinate，范围为[0,0]-[1,1]，对应如下：
    //*   [0, 0]----X----
    //*     |           |
    //*     Y           |
    //*     ----------[1, 1]
    virtual Ray sampleRay(const CameraSample &sample, const Vector2f &NDC) const = 0;

    //TODO：采样微分光线的方法
protected:
    float tNear, tFar, startTime, endTime;  //可视最近最远距离，快门开启关闭时间
};


//透视相机
//需指定相机位置、指向的点、向上方向
class PerspectiveCamera : public Camera{
public:
    // PerspectiveCamera() = delete;

    PerspectiveCamera(float _aspectRatio, float _verticalFOV = 80.f,
                      const Point3f &_position = Point3f(.0f, 2.f, 5.f),
                      const Point3f &_lookAt = Point3f(.0f, .0f, .0f),
                      const Vector3f &_up = Vector3f(.0f, 1.f, .0f));

    virtual Ray sampleRay(const CameraSample &sample, const Vector2f &NDC) const = 0;   //仍然是抽象类(分为pinhole和lens)
    
    //TODO：采样微分光线的方法

protected:
    Point3f position, lookAt;
    Vector3f up;
    float verticalFOV, aspectRatio;     //垂直FOV和屏幕长宽比
};

