#include"Parallelogram.h"

Parallelogram::Parallelogram(const Point3f &p, const Vector3f &e1, const Vector3f &e2,
                  const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian) 
                  : Shape(translate,scale,axis,radian){
    base = transform.toWorld(p);
    edge0 = transform.toWorld(e1);
    edge1 = transform.toWorld(e2);

    // TODO：计算包围盒

}

bool Parallelogram::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;
    Vector3f Paral_normal = normalize(cross(edge0, edge1));      //平行四边形法线

    //平面与光线求交算法仍需要看明白=======================
    float d = -dot(Paral_normal, p2v(base));
    float a = dot(Paral_normal, p2v(origin)) + d;
    float b = dot(Paral_normal, direction);
    if (b == .0f)
        return false; // miss
    float t = -a / b;
    //=================================================

    if(t < ray.tNear || t > ray.tFar)       //不再范围内
        return false;

    ray.tFar = t;
    intersection.t = t;
    intersection.normal = Paral_normal;
    intersection.position = origin + t * direction;
    intersection.shape = this;
    
    //纹理坐标再说(待完善)
    intersection.texCoord = Vector2f(.0f);
    
    //添加切线与副切线
    intersection.tangent = normalize(edge0);
    intersection.bitangent = normalize(cross(edge0, Paral_normal));

    return true;
}
