#include"Parallelogram.h"

Parallelogram::Parallelogram(const Point3f &p, const Vector3f &e1, const Vector3f &e2,
                  const std::shared_ptr<Light> &_light, const std::shared_ptr<Material> &_material,
                  const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian) 
                  : Shape(_material, _light, translate, scale, axis, radian){
    base = transform.toWorld(p);
    edge0 = transform.toWorld(e1);
    edge1 = transform.toWorld(e2);

    // base.debugPrint();
    // edge0.debugPrint();
    // edge1.debugPrint();
    // TODO：计算包围盒

}

bool Parallelogram::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;
    Vector3f Paral_normal = normalize(cross(edge0, edge1));      //平行四边形法线

    //平面与光线求交算法仍需要看明白=======================
    // float d = -dot(Paral_normal, p2v(base));
    // float a = dot(Paral_normal, p2v(origin)) + d;
    // float b = dot(Paral_normal, direction);
    // if (b == .0f)
    //     return false; // miss
    // float t = -a / b;       //求得光线与平面交点

    //另一种与平面求交方法：https://zhuanlan.zhihu.com/p/573076078
    float denom = dot(Paral_normal, direction);
    if(abs(denom) < 1e-6f)  //如果法向量和光方向正交，无交点
        return false;
    float t = dot(base - origin, Paral_normal) / denom;
    if(t < 0)       //小于0说明光的方向不射向平面 
        return false;
    //=================================================

    if(t < ray.tNear || t > ray.tFar)       //不在范围内
        return false;

    Point3f hitpoint = origin + t * direction;

    //计算平行四边形另外3个顶点
    Point3f p0 = base + edge0,
            p1 = base + edge1,
            p2 = base + edge0 + edge1;

    Vector3f b_p1 = p1 - base,
             p1_p2 = p2 - p1,
             p2_p0 = p0 - p2,
             p0_b = base - p0;

    //如果在平行四边形内部，则应该满足games101课上讲的叉积性质
    //如果在内部：满足该点与向量起点的向量与该向量的叉积方向都相同
    Vector3f b_h = hitpoint - base,
             p1_h = hitpoint - p1,
             p2_h = hitpoint - p2,
             p0_h = hitpoint - p0;

    Vector3f n1 = normalize(cross(b_p1, b_h)),
             n2 = normalize(cross(p1_p2, p1_h)),
             n3 = normalize(cross(p2_p0, p2_h)),
             n4 = normalize(cross(p0_b, p0_h));

    //精度问题
    if((n1 == n2) && (n1 == n3) && (n1 == n4)){
        ray.tFar = t;
        intersection.shape = this;
        intersection.normal = Paral_normal;
        intersection.position = hitpoint;
        intersection.tangent = normalize(edge0);
        intersection.bitangent = normalize(cross(intersection.tangent, intersection.normal));
        intersection.texCoord = Vector2f(.0f);      //纹理坐标再说(待完善)
        return true;
    }

    return false;

    //以下算法仍需理解===================================
    //下面的算法有问题：会沿着边无限延长，很奇怪

    // // hitpoint = base + u * e0 + v * e1, 0 <= u, v <= 1
    // Vector3f v1 = cross(hitpoint - base, edge1), v2 = cross(edge0, edge1);
    // float u_ = v1.length() / v2.length();
    // if (dot(v1, v2) < 0)
    //     u_ *= -1;

    // v1 = cross(hitpoint - base, edge0);
    // v2 = cross(edge1, edge0);
    // float v_ = v1.length() / v2.length();
    // if (dot(v1, v2) < 0)
    //     v_ *= -1;

    // //现在问题：长度对他不管用

    // if(dot(hitpoint - base, normalize(edge1)) > edge1.length() || dot(hitpoint - base, normalize(edge0)) > edge0.length())
    //     return false;

    // if (0.f <= u_ && u_ <= 1.f && 0.f <= v_ && v_ <= 1.f) {
    //     ray.tFar = t;
    //     intersection.shape = this;
    //     intersection.normal = normalize(cross(edge0, edge1));
    //     intersection.position = base + u_ * edge0 + v_ * edge1;
    //     intersection.tangent = normalize(edge0);
    //     intersection.bitangent = normalize(cross(intersection.tangent, intersection.normal));
    //     intersection.texCoord = Vector2f(u_, v_);  //纹理坐标再说(待完善)
    //     return true;
    // }

    // return false;
}


void Parallelogram::uniformSampleOnSurface(Vector2f sample, Intersection &intersection, float &pdf) const{
    float area = cross(edge0, edge1).length();     //每次调用都使用同一个即可
    pdf = 1.f / area;

    //只需要这两个有用信息即可
    intersection.normal = normalize(cross(edge0, edge1));
    intersection.position = base + sample.x * edge0 + sample.y * edge1;

    intersection.shape = this;
    intersection.t = 0;
    intersection.tangent = Vector3f(.0f);
    intersection.bitangent = Vector3f(.0f);
    intersection.texCoord = Vector2f(.0f);
}
