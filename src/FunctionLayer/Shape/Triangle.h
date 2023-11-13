#pragma once

#include"Shape.h"
#include"FunctionLayer/Light/Light.h"
#include"ResourceLayer/Mesh.h"
#include"FunctionLayer/Acceleration/BVH.h"

class TriangleMesh;

class Triangle : public Shape{
public:
    //三角形的transform默认与整个模型相同，直接赋值即可
    Triangle(int _primID, int _vtx0Idx, int _vtx1Idx, int _vtx2Idx,
         const TriangleMesh *_mesh);

    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override;

    //暂时不使用三角形面作为光源，后续完善
    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override{
        //TODO：finish this
        return;
    }

    virtual void debugPrint() const override { /*TODO*/ }

private:
    int primID;     //图元号
    int vertex0Index, vertex1Index, vertex2Index;
    std::shared_ptr<MeshData> meshData;
};


//整体模型的对象
class TriangleMesh : public Shape{
public:
    friend class Triangle;

    //暂时当成一个整体，不考虑不同部分间材质不同
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
        initInternalAcceleration();     //创建时构建内部加速结构
        innerAcceleration->shapeList.reserve(meshData->faceCount);      //提前预留好位置
    }

    //如果能够在内部直接构建一个加速结构，然后这个函数被调用时直接使用内部的加速结构进行计算就好
    virtual bool rayIntersectShape(Ray &ray, Intersection &intersection) const override{
        return innerAcceleration->rayIntersect(ray, intersection);
    }

    void initInternalAcceleration();        //模型内部的加速结构创建，同时创建整体的bounding box

    virtual void debugPrint() const override { /*TODO*/ }

    virtual void uniformSampleOnSurface(Vector2f sample, Intersection &result, float &pdf) const override {
        //TODO：finish this
        return;
    }

private:
    int maxLeafSize;
    std::shared_ptr<Acceleration> innerAcceleration;      //默认使用bvh的内部加速结构
    std::shared_ptr<MeshData> meshData;
};