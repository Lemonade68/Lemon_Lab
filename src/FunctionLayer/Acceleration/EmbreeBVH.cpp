#include"EmbreeBVH.h"

void EmbreeBVH::build() {
    //遍历shapelist中每个物体，获取其embree描述
    for(const auto shape : shapeList){
        RTCGeometry geometry = shape->getEmbreeGeometry(device);
        //添加至embree中
        rtcAttachGeometry(scene, geometry);
        rtcReleaseGeometry(geometry);       //用完立刻释放，防止占用内存
    }
    //提交当前场景，创建加速结构
    rtcCommitScene(scene);
}

//与别的加速结构一致，如果返回true，则会更新ray和intersection
bool EmbreeBVH::rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const {
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    
    RTCRayHit rtcRayHit;
    // UserRayHit1 rtcRayHit;      //添加了Intersection结构的RTCRayHit，但是对于内置类型而言无法对其中的intersection赋值

    // 初始化rtcRayHit
    rtcRayHit.ray.org_x = ray.origin[0];
    rtcRayHit.ray.org_y = ray.origin[1];
    rtcRayHit.ray.org_z = ray.origin[2];
    rtcRayHit.ray.dir_x = ray.direction[0];
    rtcRayHit.ray.dir_y = ray.direction[1];
    rtcRayHit.ray.dir_z = ray.direction[2];
    rtcRayHit.ray.tnear = ray.tNear;
    rtcRayHit.ray.tfar = ray.tFar;

    //下面不需要改变
    rtcRayHit.ray.mask = -1;
    rtcRayHit.ray.flags = 0;
    rtcRayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    //求交(因为是子类，所以仍然可以传入指针)
    //对于user定义的geometry：应该是使用自己在shape.h中定义的getEmbreeGeometry中的求交方法
    //对于内置的geometry（如三角形）：应该是使用embree内部的求交函数
    rtcIntersect1(scene, &context, &rtcRayHit);     //1: intersect a single ray with the scene

    //如果光线与加速结构没有交点
    if(rtcRayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return false;

    //有交点的话，填充数据
    ray.tFar = rtcRayHit.ray.tfar;
    *geomID = rtcRayHit.hit.geomID;
    *primID = rtcRayHit.hit.primID;
    *u = rtcRayHit.hit.u;
    *v = rtcRayHit.hit.v;
    return true;
}