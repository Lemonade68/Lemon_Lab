#include"Cube.h"

Cube::Cube(const Point3f &_boxMin, const Point3f &_boxMax,
           const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light,
           const Vector3f &_translate, const Vector3f &_scale, const Vector3f &_axis, float radian)
          : Shape(_material, _light, _translate, _scale, _axis, radian){
    //默认是中心位于(0,0,0)，棱长为2的立方体
    boxMin = _boxMin;
    boxMax = _boxMax;

    //TODO：包围盒初始化(注意不能直接将最小和最大两个点传入，可能会旋转)   ***学习这里的算法
    for (int i = 0; i < 8; ++i){
        Point3f p;
        //生成cube中的8个坐标(在局部空间中)
        p[0] = (i & 0b100) ? boxMax[0] : boxMin[0];     //2进制的i第一位是否为1
        p[1] = (i & 0b010) ? boxMax[1] : boxMin[1];     //第二位
        p[2] = (i & 0b001) ? boxMax[2] : boxMin[2];     //第三位
        
        //转换到世界空间
        p = transform.toWorld(p);

        boundingBox.Expand(p);
    }

    // 注意：AABB的计算会比较方便，因此立方体的所有计算都在局部坐标系中实现
    // 这里只对立方体进行缩放操作，而在求交部分对光线使用translate和rotate
    Matrix4f scale = transform.scalation(_scale);
    Vector4f min{boxMin[0], boxMin[1], boxMin[2], 1.f},
             max{boxMax[0], boxMax[1], boxMax[2], 1.f};     //第4个参数为1表示一个点
    min = scale * min, max = scale * max;
    min /= min[3], max /= max[3];
    boxMin = {min[0], min[1], min[2]},
    boxMax = {max[0], max[1], max[2]};
}

bool Cube::rayIntersectShape(Ray &ray, int* primID, float *u, float *v) const {
    Ray rayLocal = transform.RayToLocal(ray);      //获取变到局部坐标系中的光线，从而可以直接和AABB进行求交(注意不能直接改变光线，后面的求交会出问题)
    auto origin = rayLocal.origin;
    auto direction = rayLocal.direction;
    float tEnter = rayLocal.tNear, tExit = rayLocal.tFar;

    //进行求交，原理同AABB的rayIntersect，算法可见games101中Ray tracing一章：t = (p_x - o_x)/d_x，yz轴同理
    //* 这里如果光线的起点在立方体中（采样时可能发生），则会导致t0<0而t1>0，从而需要之后的先计算是否与远平面相交，再计算是否与近平面相交
    for (int i = 0; i < 3; ++i) {
        float invDir = 1.f / direction[i];
        float t0 = (boxMin[i] - origin[i]) * invDir,        //invDir的正负和分子的正负相同，结果一定为正
              t1 = (boxMax[i] - origin[i]) * invDir;
        if(t0 > t1)                             //始终保持t0是小的那个
            std::swap(t0, t1);
        tEnter = std::max(tEnter, t0);          //进入立方体的时间为最小值中的最大值
        tExit = std::min(tExit, t1);            //离开立方体的时间为最大值中的最小值

        if(tEnter > tExit)      //说明没有在立方体内停留
            return false;
    }

    //compute这个lambda函数通过交点信息来记录交点的面信息、在面上的偏移量
    //传[&boxMin, &boxMax]会说boxMin不是变量，原因：https://www.coder.work/article/31830
    auto compute = [min = boxMin, max = boxMax](Point3f hitpoint, int *primID, float *u, float *v){
        float minBias = FLT_MAX;        //记录最小偏移量
        int face_id = -1;               //记录打到的是哪个面

        //体会下面算法，浮点数之间直接认为没有绝对相等，因此找的是距离最小的——认为是相等
        //* 垂直于x轴的两个面，x值小的为0，x值大的为1
        //* 垂直于y轴的两个面，y值小的为2，y值大的为3
        //* 垂直于z轴的两个面，z值小的为4，z值大的为5
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
        primID = &face_id;       //记录面的信息

        //计算交点在面上的uv坐标，  **这个算法很巧妙，好好思考其与xyz轴的叉乘关系**   出自moer-lite
        //例如：垂直于x轴的面计算出来的axis就是1和2，计算的就是y和z上的比例
        //对于每个面：计算的是到左下角（两个最小值）的对应两段长度对应的uv
        int axis = (face_id / 2 + 1) % 3;
        *u = (float)(hitpoint[axis] - min[axis]) / (max[axis] - min[axis]);
        axis = (axis + 1) % 3;
        *v = (float)(hitpoint[axis] - min[axis]) / (max[axis] - min[axis]);
    };

    bool hit = false;
    // 疑问：Moer-lite中先计算tFar的意义是什么？(注释后结果确实不一样)
    //* 原因：防止光线的起点在正方体中时，tEnter<0直接跳过下面的if，从而认为采样光线不与物体相交的问题
    //* 思考见日志第11条
    if(ray.tNear < tExit && tExit < ray.tFar){
        Point3f hitpoint = origin + tExit * direction;
        compute(hitpoint, primID, u, v);
        ray.tFar = tExit;
        hit = true;
    }

    //如果tEnter是真正的击中距离，overwrite上面的结果
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

    //将本地的法线转换为世界空间的法线
    Vector4f normal(.0f);
    normal[primID / 2] = (primID % 2) ? 1 : -1;
    normal = transform.rotate * normal;     //平移和缩放不改变法线方向
    intersection->normal = normalize(Vector3f(normal[0], normal[1], normal[2]));

    //将本地交点坐标转换为世界空间的交点
    Vector4f hitpoint;
    //  计算本地坐标
    hitpoint[primID / 2] = (primID % 2) ? boxMax[primID / 2] : boxMin[primID / 2];     //设置该平面垂直的轴对应的值
    int axis = (primID / 2 + 1) % 3;        //计算u对应的位置坐标
    hitpoint[axis] = boxMin[axis] + u * (boxMax[axis] - boxMin[axis]);
    axis = (axis + 1) % 3;                //计算v对应的位置坐标
    hitpoint[axis] = boxMin[axis] + v * (boxMax[axis] - boxMin[axis]);
    hitpoint[3] = 1.f;
    //  转换成世界坐标(初始化时已经缩放过了)
    hitpoint = transform.translate * transform.rotate * hitpoint;       //先旋转，后平移
    hitpoint /= hitpoint[3];
    intersection->position = Point3f(hitpoint[0], hitpoint[1], hitpoint[2]);

    //计算切线和副切线
    Vector3f tangent(1.f, .0f, .0f);
    Vector3f bitangent;
    if(std::abs(dot(tangent,intersection->normal)) > 0.9f)   //距离过近，当做平行，做不了cross，换个数
        tangent = Vector3f(.0f, 1.f, .0f);
    bitangent = normalize(cross(tangent, intersection->normal));
    tangent = normalize(cross(intersection->normal, bitangent));
    intersection->bitangent = bitangent;
    intersection->tangent = tangent;
}