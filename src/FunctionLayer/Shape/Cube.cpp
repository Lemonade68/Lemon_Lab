#include"Cube.h"

Cube::Cube(const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light,
         const Point3f &_boxMin, const Point3f &_boxMax,
         const Vector3f &_translate, const Vector3f &_scale, const Vector3f &_axis, float radian)
         : Shape(_material, _light, _translate, _scale, _axis, radian){
    //默认是中心位于(0,0,0)，棱长为2的立方体
    boxMin = _boxMin;
    boxMax = _boxMax;

    //TODO：包围盒初始化

    //注意：AABB的计算会比较方便，因此立方体的所有计算都在局部坐标系中实现
    //这里只对立方体进行缩放操作，而在求交部分对光线使用translate和rotate
    Matrix4f scale = transform.scalation(_scale);
    Vector4f min{boxMin[0], boxMin[1], boxMin[2], 1.f},
             max{boxMax[0], boxMax[1], boxMax[2], 1.f};     //第4个参数为1表示一个点
    min = scale * min, max = scale * max;
    min /= min[3], max /= max[3];
    boxMin = {min[0], min[1], min[2]},
    boxMax = {max[0], max[1], max[2]};
}


bool Cube::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    Ray rayLocal = transform.RayToLocal(ray);      //获取变到局部坐标系中的光线，从而可以直接和AABB进行求交(注意不能直接改变光线，后面的求交会出问题)
    auto origin = rayLocal.origin;
    auto direction = rayLocal.direction;
    float tEnter = rayLocal.tNear, tExit = rayLocal.tFar;

    //进行求交，原理同AABB的rayIntersect，算法可见games101中Ray tracing一章：t = (p_x - o_x)/d_x，yz轴同理
    for (int i = 0; i < 3; ++i) {
        float invDir = 1.f / direction[i];
        float t0 = (boxMin[i] - origin[i]) * invDir,
              t1 = (boxMax[i] - origin[i]) * invDir;
        if(t0 > t1)
            std::swap(t0, t1);
        tEnter = std::max(tEnter, t0);          //进入立方体的时间为最小值中的最大值
        tExit = std::min(tExit, t1);            //离开立方体的时间为最大值中的最小值

        if(tEnter > tExit)      //说明没有在立方体内停留
            return false;
    }

    


}