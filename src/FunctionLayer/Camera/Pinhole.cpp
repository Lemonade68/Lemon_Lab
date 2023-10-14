#include"Pinhole.h"

PinholeCamera::PinholeCamera(float _aspectRatio, float _verticalFOV,
                             const Point3f &_position, const Point3f &_lookAt, const Vector3f &_up)
    : PerspectiveCamera(_aspectRatio, _verticalFOV, _position, _lookAt, _up){}


Ray PinholeCamera::sampleRay(const CameraSample &sample, const Vector2f &NDC) const{
    //-------------相机空间下------------
    //画图，将NDC的xy坐标都转换为(-0.5, 0.5)的范围，坐标轴在屏幕中心，注意两者与0.5的减法顺序，后乘上对应分辨率
    float x = (NDC[0] - 0.5f) * 1200.f + sample.xy[0];         //这里1200后面改()
    float y = (0.5f - NDC[1]) * 800.f + sample.xy[1];

    float tanHalfFov = std::tan(verticalFOV * 0.5f);
    float z = -800.f * 0.5f / tanHalfFov;       //相机到近平面的距离，注意负号(光线的坐标系的z是沿着-forward方向的)

    Vector3f direction = Vector3f(x, y, z);     //相机坐标系下的光线方向

    //----------变换到世界空间下----------

    direction = normalize(transform.toWorld(direction));
    Point3f origin = transform.toWorld(Point3f(.0f));       //光线起点就是摄像机的位置

    return Ray(origin, direction, tNear, tFar, startTime);
}

//TODO：微分光线部分

