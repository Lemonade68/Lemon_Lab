#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"

class AABB{
public:
    AABB(){
        //初始化时将最小值设置为最大，最大值设置为最小，任何改变都可以直接改变包围盒
        pMin = Point3f(FLT_MAX, FLT_MAX, FLT_MAX);
        pMax = Point3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }

    AABB(const Point3f &_pMin, const Point3f &_pMax) : pMin(_pMin), pMax(_pMax) {}

    //合并两个包围盒，并返回一个新的AABB
    AABB Union(const AABB &other) const;

    //在原来的AABB上扩张（融合另一个AABB）
    void Expand(const AABB &other);

    //通过一个点来返回新的AABB，使其包含该点
    AABB Union(const Point3f &other) const;

    //在原来的AABB上扩张（包含另一个点）
    void Expand(const Point3f &other);

    //判断当前包围盒是否包含other包围盒
    bool Overlap(const AABB &other) const;

    //判断光线是否与包围盒有交点，返回进入和离开包围盒的时间
    bool RayIntersect(Ray &ray, float *tMin = nullptr, float *tMax = nullptr) const;

    //返回包围盒中心（包围盒生成算法中使用）
    Point3f Center() const;

    void debugPrint() const{
        std::cout << "Bounding box  ——  pMin: ";
        pMin.debugPrint();
        std::cout << "pMax: ";
        pMax.debugPrint();
    }

public:
    Point3f pMin, pMax;
};