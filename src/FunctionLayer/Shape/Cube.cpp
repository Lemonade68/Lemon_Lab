#include"Cube.h"

Cube::Cube(const Point3f &_boxMin, const Point3f &_boxMax,
           const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light,
           const Vector3f &_translate, const Vector3f &_scale, const Vector3f &_axis, float radian)
          : Shape(_material, _light, _translate, _scale, _axis, radian){
    //Ĭ��������λ��(0,0,0)���ⳤΪ2��������
    boxMin = _boxMin;
    boxMax = _boxMax;

    //TODO����Χ�г�ʼ��(ע�ⲻ��ֱ�ӽ���С����������㴫�룬���ܻ���ת)   ***ѧϰ������㷨
    for (int i = 0; i < 8; ++i){
        Point3f p;
        //����cube�е�8������(�ھֲ��ռ���)
        p[0] = (i & 0b100) ? boxMax[0] : boxMin[0];     //2���Ƶ�i��һλ�Ƿ�Ϊ1
        p[1] = (i & 0b010) ? boxMax[1] : boxMin[1];     //�ڶ�λ
        p[2] = (i & 0b001) ? boxMax[2] : boxMin[2];     //����λ
        
        //ת��������ռ�
        p = transform.toWorld(p);

        boundingBox.Expand(p);
    }

    // ע�⣺AABB�ļ����ȽϷ��㣬�������������м��㶼�ھֲ�����ϵ��ʵ��
    // ����ֻ��������������Ų����������󽻲��ֶԹ���ʹ��translate��rotate
    Matrix4f scale = transform.scalation(_scale);
    Vector4f min{boxMin[0], boxMin[1], boxMin[2], 1.f},
             max{boxMax[0], boxMax[1], boxMax[2], 1.f};     //��4������Ϊ1��ʾһ����
    min = scale * min, max = scale * max;
    min /= min[3], max /= max[3];
    boxMin = {min[0], min[1], min[2]},
    boxMax = {max[0], max[1], max[2]};
}

bool Cube::rayIntersectShape(Ray &ray, int* primID, float *u, float *v) const {
    Ray rayLocal = transform.RayToLocal(ray);      //��ȡ�䵽�ֲ�����ϵ�еĹ��ߣ��Ӷ�����ֱ�Ӻ�AABB������(ע�ⲻ��ֱ�Ӹı���ߣ�������󽻻������)
    auto origin = rayLocal.origin;
    auto direction = rayLocal.direction;
    float tEnter = rayLocal.tNear, tExit = rayLocal.tFar;

    //�����󽻣�ԭ��ͬAABB��rayIntersect���㷨�ɼ�games101��Ray tracingһ�£�t = (p_x - o_x)/d_x��yz��ͬ��
    //* ����������ߵ�������������У�����ʱ���ܷ���������ᵼ��t0<0��t1>0���Ӷ���Ҫ֮����ȼ����Ƿ���Զƽ���ཻ���ټ����Ƿ����ƽ���ཻ
    for (int i = 0; i < 3; ++i) {
        float invDir = 1.f / direction[i];
        float t0 = (boxMin[i] - origin[i]) * invDir,        //invDir�������ͷ��ӵ�������ͬ�����һ��Ϊ��
              t1 = (boxMax[i] - origin[i]) * invDir;
        if(t0 > t1)                             //ʼ�ձ���t0��С���Ǹ�
            std::swap(t0, t1);
        tEnter = std::max(tEnter, t0);          //�����������ʱ��Ϊ��Сֵ�е����ֵ
        tExit = std::min(tExit, t1);            //�뿪�������ʱ��Ϊ���ֵ�е���Сֵ

        if(tEnter > tExit)      //˵��û������������ͣ��
            return false;
    }

    //compute���lambda����ͨ��������Ϣ����¼���������Ϣ�������ϵ�ƫ����
    //��[&boxMin, &boxMax]��˵boxMin���Ǳ�����ԭ��https://www.coder.work/article/31830
    auto compute = [min = boxMin, max = boxMax](Point3f hitpoint, int *primID, float *u, float *v){
        float minBias = FLT_MAX;        //��¼��Сƫ����
        int face_id = -1;               //��¼�򵽵����ĸ���

        //��������㷨��������֮��ֱ����Ϊû�о�����ȣ�����ҵ��Ǿ�����С�ġ�����Ϊ�����
        //* ��ֱ��x��������棬xֵС��Ϊ0��xֵ���Ϊ1
        //* ��ֱ��y��������棬yֵС��Ϊ2��yֵ���Ϊ3
        //* ��ֱ��z��������棬zֵС��Ϊ4��zֵ���Ϊ5
        for (int i = 0; i < 3; ++i){
            if (float bias = std::abs(hitpoint[i] - min[i]); bias < minBias){
                face_id = 2 * i;
                minBias = bias;
            }
            if (float bias = std::abs(hitpoint[i] - max[i]); bias < minBias){
                face_id = 2 * i + 1;
                minBias = bias;
            }
        }
        primID = &face_id;       //��¼�����Ϣ

        //���㽻�������ϵ�uv���꣬  **����㷨������ú�˼������xyz��Ĳ�˹�ϵ**   ����moer-lite
        //���磺��ֱ��x�������������axis����1��2������ľ���y��z�ϵı���
        //����ÿ���棺������ǵ����½ǣ�������Сֵ���Ķ�Ӧ���γ��ȶ�Ӧ��uv
        int axis = (face_id / 2 + 1) % 3;
        *u = (float)(hitpoint[axis] - min[axis]) / (max[axis] - min[axis]);
        axis = (axis + 1) % 3;
        *v = (float)(hitpoint[axis] - min[axis]) / (max[axis] - min[axis]);
    };

    bool hit = false;
    // ���ʣ�Moer-lite���ȼ���tFar��������ʲô��(ע�ͺ���ȷʵ��һ��)
    //* ԭ�򣺷�ֹ���ߵ��������������ʱ��tEnter<0ֱ�����������if���Ӷ���Ϊ�������߲��������ཻ������
    //* ˼������־��11��
    if(ray.tNear < tExit && tExit < ray.tFar){
        Point3f hitpoint = origin + tExit * direction;
        compute(hitpoint, primID, u, v);
        ray.tFar = tExit;
        hit = true;
    }

    //���tEnter�������Ļ��о��룬overwrite����Ľ��
    if(ray.tNear < tEnter && tEnter < ray.tFar){
        Point3f hitpoint = origin + tEnter * direction;
        compute(hitpoint, primID, u, v);
        ray.tFar = tEnter;
        hit = true;
    }
    return hit;
}

void Cube::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const {
    intersection->shape = this;
    intersection->t = tFar;
    intersection->texCoord = Vector2f(u, v);

    //�����صķ���ת��Ϊ����ռ�ķ���
    Vector4f normal(.0f);
    normal[primID / 2] = (primID % 2) ? 1 : -1;
    normal = transform.rotate * normal;     //ƽ�ƺ����Ų��ı䷨�߷���
    intersection->normal = normalize(Vector3f(normal[0], normal[1], normal[2]));

    //�����ؽ�������ת��Ϊ����ռ�Ľ���
    Vector4f hitpoint;
    //  ���㱾������
    hitpoint[primID / 2] = (primID % 2) ? boxMax[primID / 2] : boxMin[primID / 2];     //���ø�ƽ�洹ֱ�����Ӧ��ֵ
    int axis = (primID / 2 + 1) % 3;        //����u��Ӧ��λ������
    hitpoint[axis] = boxMin[axis] + u * (boxMax[axis] - boxMin[axis]);
    axis = (axis + 1) % 3;                //����v��Ӧ��λ������
    hitpoint[axis] = boxMin[axis] + v * (boxMax[axis] - boxMin[axis]);
    hitpoint[3] = 1.f;
    //  ת������������(��ʼ��ʱ�Ѿ����Ź���)
    hitpoint = transform.translate * transform.rotate * hitpoint;       //����ת����ƽ��
    hitpoint /= hitpoint[3];
    intersection->position = Point3f(hitpoint[0], hitpoint[1], hitpoint[2]);

    //�������ߺ͸�����
    Vector3f tangent(1.f, .0f, .0f);
    Vector3f bitangent;
    if(std::abs(dot(tangent,intersection->normal)) > 0.9f)   //�������������ƽ�У�������cross��������
        tangent = Vector3f(.0f, 1.f, .0f);
    bitangent = normalize(cross(tangent, intersection->normal));
    tangent = normalize(cross(intersection->normal, bitangent));
    intersection->bitangent = bitangent;
    intersection->tangent = tangent;
}