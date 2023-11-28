#pragma once

#include"Acceleration.h"
#include"embree3/rtcore.h"

inline void errorFunction(void *userPtr, enum RTCError error, const char *str){
    std::cerr << "error " << error << ": " << str << std::endl;
}

class EmbreeBVH : public Acceleration{
public:
    EmbreeBVH(): Acceleration(){
        //初始化embree
        device = rtcNewDevice(nullptr);
        rtcSetDeviceErrorFunction(device, errorFunction, NULL);
        scene = rtcNewScene(device);
    }

    EmbreeBVH(const std::vector<std::shared_ptr<Shape>> &_shapeList) : Acceleration(_shapeList) { 
        //初始化Embree
        device = rtcNewDevice(nullptr);
        rtcSetDeviceErrorFunction(device, errorFunction, NULL);
        scene = rtcNewScene(device);
    }

    virtual ~EmbreeBVH() = default;



    virtual void build() override;

    virtual void debugPrint() const override{
        //TODO
    }

    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const override;

private:
    RTCDevice device;   // device object，用于创建各种相关对象
    RTCScene scene;     // scene object，用于包含场景中各个物体
};