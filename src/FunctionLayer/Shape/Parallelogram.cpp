#include"Parallelogram.h"

Parallelogram::Parallelogram(const Point3f &p, const Vector3f &e1, const Vector3f &e2,
                  const std::shared_ptr<Light> &_light, const std::shared_ptr<Material> &_material,
                  const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian) 
                  : Shape(_material, _light, translate, scale, axis, radian){
    base = transform.toWorld(p);
    edge0 = transform.toWorld(e1);
    edge1 = transform.toWorld(e2);

    // TODO�������Χ��
    Point3f vertices[4];
    vertices[0] = base;
    vertices[1] = base + edge0;
    vertices[2] = base + edge1;
    vertices[3] = vertices[2] + edge0;

    for (int i = 0; i < 4; ++i)
        boundingBox.Expand(vertices[i]);
}

bool Parallelogram::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const{
    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;
    Vector3f Paral_normal = normalize(cross(edge0, edge1));      //ƽ���ı��η���

    //ƽ����������㷨����Ҫ������=======================
    // float d = -dot(Paral_normal, p2v(base));
    // float a = dot(Paral_normal, p2v(origin)) + d;
    // float b = dot(Paral_normal, direction);
    // if (b == .0f)
    //     return false; // miss
    // float t = -a / b;       //��ù�����ƽ�潻��

    //��һ����ƽ���󽻷�����https://zhuanlan.zhihu.com/p/573076078
    float denom = dot(Paral_normal, direction);
    if(abs(denom) < 1e-6f)  //����������͹ⷽ���������޽���
        return false;
    float t = dot(base - origin, Paral_normal) / denom;
    if(t < 0)               //С��0˵����ķ�������ƽ�� 
        return false;
    //=================================================

    if(t < ray.tNear || t > ray.tFar)       //���ڷ�Χ��
        return false;

    Point3f hitpoint = origin + t * direction;

    //����ƽ���ı�������3������
    Point3f p0 = base + edge0,
            p1 = base + edge1,
            p2 = base + edge0 + edge1;

    Vector3f b_p1 = p1 - base,
             p1_p2 = p2 - p1,
             p2_p0 = p0 - p2,
             p0_b = base - p0;

    //�����ƽ���ı����ڲ�����Ӧ������games101���Ͻ��Ĳ������
    //������ڲ�������õ�����������������������Ĳ��������ͬ
    Vector3f b_h = hitpoint - base,
             p1_h = hitpoint - p1,
             p2_h = hitpoint - p2,
             p0_h = hitpoint - p0;

    Vector3f n1 = normalize(cross(b_p1, b_h)),
             n2 = normalize(cross(p1_p2, p1_h)),
             n3 = normalize(cross(p2_p0, p2_h)),
             n4 = normalize(cross(p0_b, p0_h));

    //��������(==����������ض���)
    if((n1 == n2) && (n1 == n3) && (n1 == n4)){
        ray.tFar = t;
        Vector3f e0_norm = normalize(edge0),
                 e1_norm = normalize(edge1),
                 hitpoint2base = hitpoint - base;
        *u = dot(hitpoint2base, e0_norm) / edge0.length();
        *v = dot(hitpoint2base, e1_norm) / edge1.length();
        *primID = 0;
        return true;
    }

    return false;

    //�����㷨�������===================================
    //������㷨�����⣺�����ű������ӳ��������  ����  length���������⣬sphereͬ��

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

    // //�������⣺���ȶ���������

    // if(dot(hitpoint - base, normalize(edge1)) > edge1.length() || dot(hitpoint - base, normalize(edge0)) > edge0.length())
    //     return false;

    // if (0.f <= u_ && u_ <= 1.f && 0.f <= v_ && v_ <= 1.f) {
    //     ray.tFar = t;
    //     *primID = 0;
    //     *u = u_;
    //     *v = v_;
    //     return true;
    // }

    // return false;
}

void Parallelogram::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const {
    intersection->shape = this;
    intersection->t = tFar;
    intersection->position = base + u * edge0 + v * edge1;
    intersection->normal = normalize(cross(edge0, edge1));
    intersection->texCoord = Vector2f{u, v};
    intersection->tangent = normalize(edge0);
    intersection->bitangent = normalize(cross(intersection->tangent, intersection->normal));
}



void Parallelogram::uniformSampleOnSurface(Vector2f sample, Intersection &intersection, float &pdf) const{
    float area = cross(edge0, edge1).length();     //ÿ�ε��ö�ʹ��ͬһ������
    pdf = 1.f / area;

    //ֻ��Ҫ������������Ϣ����
    intersection.normal = normalize(cross(edge0, edge1));
    intersection.position = base + sample.x * edge0 + sample.y * edge1;

    intersection.shape = this;
    intersection.t = 0;
    intersection.tangent = Vector3f(.0f);
    intersection.bitangent = Vector3f(.0f);
    intersection.texCoord = Vector2f(.0f);
}
