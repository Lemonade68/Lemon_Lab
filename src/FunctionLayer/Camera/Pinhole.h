#pragma once

#include"Camera.h"
class PinholeCamera : public PerspectiveCamera {
public:
    // PinholeCamera() = delete;

    PinholeCamera(float _aspectRatio, float _verticalFOV = 80.f,
                  const Point3f &_position = Point3f(.0f, .5f, 5.f),
                  const Point3f &_lookAt = Point3f(.0f, .0f, .0f),
                  const Vector3f &_up = Vector3f(.0f, 1.f, .0f));   // 直接默认用Camera的默认构造函数了

    virtual Ray sampleRay(const CameraSample &sample, const Vector2f &NDC) const override;

    void debugPrint() const{
        std::cout << aspectRatio << ' ' << verticalFOV << '\n';
        position.debugPrint();
        lookAt.debugPrint();
        up.debugPrint();
    }

    //TODO：微分光线的sampleRay
};