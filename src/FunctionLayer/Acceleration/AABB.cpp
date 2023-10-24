#include"AABB.h"

//工具函数(取两点各分量的最小/大坐标生成新的点)
Point3f minP(const Point3f &p1, const Point3f &p2){
    return Point3f(std::min(p1[0], p2[0]), std::min(p1[1], p2[1]), std::min(p1[2], p2[2]));
}

Point3f maxP(const Point3f &p1, const Point3f &p2){
    return Point3f(std::max(p1[0], p2[0]), std::max(p1[1], p2[1]), std::max(p1[2], p2[2]));
}


AABB AABB::Union(const AABB &other) const{
    return AABB(minP(pMin, other.pMin), maxP(pMax, other.pMax));
}

void AABB::Expand(const AABB &other){
    pMin = minP(pMin, other.pMin);
    pMax = maxP(pMax, other.pMax);
}

AABB AABB::Union(const Point3f &other) const{
    return AABB(minP(pMin, other), maxP(pMax, other));
}

void AABB::Expand(const Point3f &other){
    pMin = minP(pMin, other);
    pMax = maxP(pMax, other);
}

bool AABB::Overlap(const AABB &other) const{
    for (int axis = 0; axis < 3; ++axis){
        if(pMin[axis] > other.pMin[axis] || pMax[axis] < other.pMax[axis])
            return false;
    }
    return true;
}

//从物体中间发射光线穿出的情况也会返回true，只不过tMin小于0，tMax大于0
//这里整体的求交过程与Cube中无异
bool AABB::RayIntersect(Ray &ray, float *tMin, float *tMax) const{
    float tEnter = ray.tNear, tExit = ray.tFar;
    for (int i = 0; i < 3; ++i){
        float invDir = 1.f / ray.direction[i];
        float t0 = (pMin[i] - ray.origin[i]) * invDir,
              t1 = (pMax[i] - ray.origin[i]) * invDir;
        if(t0 > t1)
            std::swap(t0, t1);
        tEnter = std::max(tEnter, t0);
        tExit = std::min(tExit, t1);
        if(tEnter > tExit){
            return false;
        }
    }
    if(tMin)
        *tMin = tEnter;
    if(tMax)
        *tMax = tExit;

    return true;
}


Point3f AABB::Center() const{
    return Point3f{(pMin[0] + pMax[0]) * .5f, (pMin[1] + pMax[1]) * .5f,
                   (pMin[2] + pMax[2]) * .5f};
}
