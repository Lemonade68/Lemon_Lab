#pragma once

#include"Acceleration.h"
#include"embree3/rtcore.h"

class EmbreeBVH : public Acceleration{
public:
    EmbreeBVH(): Acceleration(){
        //��ʼ��embree
        device = rtcNewDevice(nullptr);
        scene = rtcNewScene(device);
    }

    EmbreeBVH(const std::vector<std::shared_ptr<Shape>> &_shapeList) : Acceleration(_shapeList) { 
        //��ʼ��Embree
        device = rtcNewDevice(nullptr);
        scene = rtcNewScene(device);
    }

    virtual ~EmbreeBVH() = default;

    virtual void build() override;

    virtual void debugPrint() const override{
        //TODO
    }

    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const override;

private:
    RTCDevice device;   // device object�����ڴ���������ض���
    RTCScene scene;     // scene object�����ڰ��������и�������
};