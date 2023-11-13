#pragma once

#include"Shape.h"
#include"FunctionLayer/Light/Light.h"
#include"ResourceLayer/Mesh.h"
#include"FunctionLayer/Acceleration/BVH.h"

class TriangleMesh;

class Triangle : public Shape{
public:
    //�����ε�transformĬ��������ģ����ͬ��ֱ�Ӹ�ֵ����
    Triangle(int _primID, int _vtx0Idx, int _vtx1Idx, int _vtx2Idx,
         const TriangleMesh *_mesh);

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    //��ʱ��ʹ������������Ϊ��Դ����������
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override{
        //TODO��finish this
        return;
    }

    virtual void debugPrint() const override { /*TODO*/ }

private:
    int primID;     //ͼԪ��
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
        innerAcceleration->shapeList.reserve(meshData->faceCount);      //��ǰԤ����λ��
    }

    //����ܹ����ڲ�ֱ�ӹ���һ�����ٽṹ��Ȼ���������������ʱֱ��ʹ���ڲ��ļ��ٽṹ���м���ͺ�
    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override{
        return innerAcceleration->rayIntersect(ray, intersection);
    }

    void initInternalAcceleration();        //ģ���ڲ��ļ��ٽṹ������ͬʱ���������bounding box

    virtual void debugPrint() const override { /*TODO*/ }

    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override {
        //TODO��finish this
        return;
    }

private:
    int maxLeafSize;
    std::shared_ptr<Acceleration> innerAcceleration;      //Ĭ��ʹ��bvh���ڲ����ٽṹ
    std::shared_ptr<MeshData> meshData;
};