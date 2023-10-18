#include"Cube.h"

Cube::Cube(const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light,
         const Point3f &_boxMin, const Point3f &_boxMax,
         const Vector3f &_translate, const Vector3f &_scale, const Vector3f &_axis, float radian)
         : Shape(_material, _light, _translate, _scale, _axis, radian){
    //Ĭ��������λ��(0,0,0)���ⳤΪ2��������
    boxMin = _boxMin;
    boxMax = _boxMax;

    //TODO����Χ�г�ʼ��

    //ע�⣺AABB�ļ����ȽϷ��㣬�������������м��㶼�ھֲ�����ϵ��ʵ��
    //����ֻ��������������Ų����������󽻲��ֶԹ���ʹ��translate��rotate
    Matrix4f scale = transform.scalation(_scale);
    Vector4f min{boxMin[0], boxMin[1], boxMin[2], 1.f},
             max{boxMax[0], boxMax[1], boxMax[2], 1.f};     //��4������Ϊ1��ʾһ����
    min = scale * min, max = scale * max;
    min /= min[3], max /= max[3];
    boxMin = {min[0], min[1], min[2]},
    boxMax = {max[0], max[1], max[2]};
}


bool Cube::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    Ray rayLocal = transform.RayToLocal(ray);      //��ȡ�䵽�ֲ�����ϵ�еĹ��ߣ��Ӷ�����ֱ�Ӻ�AABB������(ע�ⲻ��ֱ�Ӹı���ߣ�������󽻻������)
    auto origin = rayLocal.origin;
    auto direction = rayLocal.direction;
    float tEnter = rayLocal.tNear, tExit = rayLocal.tFar;

    //�����󽻣�ԭ��ͬAABB��rayIntersect���㷨�ɼ�games101��Ray tracingһ�£�t = (p_x - o_x)/d_x��yz��ͬ��
    for (int i = 0; i < 3; ++i) {
        float invDir = 1.f / direction[i];
        float t0 = (boxMin[i] - origin[i]) * invDir,
              t1 = (boxMax[i] - origin[i]) * invDir;
        if(t0 > t1)
            std::swap(t0, t1);
        tEnter = std::max(tEnter, t0);          //�����������ʱ��Ϊ��Сֵ�е����ֵ
        tExit = std::min(tExit, t1);            //�뿪�������ʱ��Ϊ���ֵ�е���Сֵ

        if(tEnter > tExit)      //˵��û������������ͣ��
            return false;
    }

    


}