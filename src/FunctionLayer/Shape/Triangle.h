#pragma once

#include"Shape.h"
#include"FunctionLayer/Light/Light.h"
#include"ResourceLayer/Mesh.h"
#include"FunctionLayer/Acceleration/BVH.h"
#include"FunctionLayer/Acceleration/EmbreeBVH.h"

class TriangleMesh;

class Triangle : public Shape{
public:
    //�����ε�transformĬ��������ģ����ͬ��ֱ�Ӹ�ֵ����
    Triangle(int _primID, int _vtx0Idx, int _vtx1Idx, int _vtx2Idx,
         const TriangleMesh *_mesh);

    virtual bool rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const override;

    virtual void fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const override;

    //��ʱ��ʹ������������Ϊ��Դ����������
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override{
        //TODO��finish this
        return;
    }

    virtual void debugPrint() const override { /*TODO*/ }

    //* �������������㷨
    // virtual RTCGeometry getEmbreeGeometry(RTCDevice device) const override;


private:
    int primID;     //ͼԪ�ţ���mesh�У�
    int vertex0Index, vertex1Index, vertex2Index;
    std::shared_ptr<MeshData> meshData;
};


//����ģ�͵Ķ���
class TriangleMesh : public Shape{
public:
    friend class Triangle;

    //��ʱ����һ�����壬�����ǲ�ͬ���ּ���ʲ�ͬ
    TriangleMesh(std::string filepath, int _maxLeafSize = 5, 
         const std::shared_ptr<Material> _material = std::make_shared<Matte_Material>(),
         const std::shared_ptr<Light> _light = nullptr,
         const Vector3f &_translate = Vector3f(.0f),
         const Vector3f &_scale = Vector3f(1.f),
         const Vector3f &_axis = Vector3f(.0f, 1.f, .0f),
         float radian = .0f) : Shape(_material, _light,_translate,_scale,_axis,radian) {

        maxLeafSize = _maxLeafSize;
        auto _mesh = MeshData::loadFromFile(filepath);
        meshData = _mesh;
        initInternalAcceleration();     //����ʱ�����ڲ����ٽṹ
    }

    //����ܹ����ڲ�ֱ�ӹ���һ�����ٽṹ��Ȼ���������������ʱֱ��ʹ���ڲ��ļ��ٽṹ���м���ͺ�
    //* ��ʹ��embree����ʱ���÷������ᱻ����
    virtual bool rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const override;

    virtual void fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const override;

    void initInternalAcceleration();        //ģ���ڲ��ļ��ٽṹ������ͬʱ���������bounding box

    virtual void debugPrint() const override { /*TODO*/ }

    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override {
        //TODO��finish this
        return;
    }

    //����triangle mesh���ж��primitives��Ϊ�������ͣ������Ҫ���¶���getEmbreeGeometry������
    //����Ҳ��embree�ڲ���ɣ��Ӷ���ʹ��embreeʱ�����������������йز���ȫ����Embree�����ú����ṩ����Ϣ���
    //����������غ���ʧЧ
    virtual RTCGeometry getEmbreeGeometry(RTCDevice device) const override;

private:
    int maxLeafSize;            //��BVHʹ��
    std::shared_ptr<Acceleration> innerAcceleration;      //Ĭ��ʹ��bvh���ڲ����ٽṹ
    std::shared_ptr<MeshData> meshData;
};