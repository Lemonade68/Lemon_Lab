#include"EmbreeBVH.h"

void EmbreeBVH::build() {
    //����shapelist��ÿ�����壬��ȡ��embree����
    for(const auto shape : shapeList){
        RTCGeometry geometry = shape->getEmbreeGeometry(device);
        //�����embree��
        rtcAttachGeometry(scene, geometry);
        rtcReleaseGeometry(geometry);       //���������ͷţ���ֹռ���ڴ�
    }
    //�ύ��ǰ�������������ٽṹ
    rtcCommitScene(scene);
}

//���ļ��ٽṹһ�£��������true��������ray��intersection
bool EmbreeBVH::rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const {
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    
    RTCRayHit rtcRayHit;
    // UserRayHit1 rtcRayHit;      //�����Intersection�ṹ��RTCRayHit�����Ƕ����������Ͷ����޷������е�intersection��ֵ

    // ��ʼ��rtcRayHit
    rtcRayHit.ray.org_x = ray.origin[0];
    rtcRayHit.ray.org_y = ray.origin[1];
    rtcRayHit.ray.org_z = ray.origin[2];
    rtcRayHit.ray.dir_x = ray.direction[0];
    rtcRayHit.ray.dir_y = ray.direction[1];
    rtcRayHit.ray.dir_z = ray.direction[2];
    rtcRayHit.ray.tnear = ray.tNear;
    rtcRayHit.ray.tfar = ray.tFar;

    //���治��Ҫ�ı�
    rtcRayHit.ray.mask = -1;
    rtcRayHit.ray.flags = 0;
    rtcRayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    //��(��Ϊ�����࣬������Ȼ���Դ���ָ��)
    //����user�����geometry��Ӧ����ʹ���Լ���shape.h�ж����getEmbreeGeometry�е��󽻷���
    //�������õ�geometry���������Σ���Ӧ����ʹ��embree�ڲ����󽻺���
    rtcIntersect1(scene, &context, &rtcRayHit);     //1: intersect a single ray with the scene

    //�����������ٽṹû�н���
    if(rtcRayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID)
        return false;

    //�н���Ļ����������
    ray.tFar = rtcRayHit.ray.tfar;
    *geomID = rtcRayHit.hit.geomID;
    *primID = rtcRayHit.hit.primID;
    *u = rtcRayHit.hit.u;
    *v = rtcRayHit.hit.v;
    return true;
}