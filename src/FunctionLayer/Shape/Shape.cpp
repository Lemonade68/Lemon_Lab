#include"Shape.h"
#include"embree3/rtcore_geometry.h"

//初始化物体时需要将其transform初始化
Shape::Shape(const std::shared_ptr<Material> &_material, const std::shared_ptr<Light> &_light, 
             const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian){
    //获取矩阵描述
    Matrix4f tranlsateMat = Transform::translation(translate);
    Matrix4f scaleMat = Transform::scalation(scale);
    Matrix4f rotateMat = Transform::rotation(axis, radian);

    transform = Transform(tranlsateMat, rotateMat, scaleMat);

    material = _material;
    light = _light;
}

//TODO:后续东西添加


// sets a callback to query the bounding box of user-defined primitives
void UserShapeBound(const RTCBoundsFunctionArguments *args) {
    Shape *shape = static_cast<Shape *>(args->geometryUserPtr);
    auto [pMin, pMax] = shape->getAABB();       //c++17：结构化绑定声明
    args->bounds_o->lower_x = pMin[0];
    args->bounds_o->lower_y = pMin[1];
    args->bounds_o->lower_z = pMin[2];

    args->bounds_o->upper_x = pMax[0];
    args->bounds_o->upper_y = pMax[1];
    args->bounds_o->upper_z = pMax[2];
}

// sets the callback function to intersect a user geometry
void UserShapeIntersect(const RTCIntersectFunctionNArguments *args){
    int *valid = args->valid;
    if(!valid[0])
        return;

    Shape *shape = static_cast<Shape *>(args->geometryUserPtr);
    // RTCRayHit *rayhit = static_cast<RTCRayHit *>(args->rayhit);  //显示转换类型无效
    RTCRayHit *rayhit = (RTCRayHit *)args->rayhit;      //记录ray和hit的信息
    // UserRayHit1 *rayhit = (UserRayHit1 *)(args->rayhit);    //添加intersection结构的RTCRayHit

    //根据rayhit中ray的信息转换为自己定义的光线
    Point3f origin{rayhit->ray.org_x, rayhit->ray.org_y, rayhit->ray.org_z};
    Vector3f direction{rayhit->ray.dir_x, rayhit->ray.dir_y, rayhit->ray.dir_z};
    Ray ray{origin, direction, 1e-4f, rayhit->ray.tfar};

    //向rayhit中的hit记录信息
    int primID;
    float u, v;
    bool hit = shape->rayIntersectShape(ray, &primID, &u, &v);
    if(hit) {   //如果击中
        rayhit->ray.tfar = ray.tFar;    //若击中，更新rayhit中的tfar为击中后光线的tfar
        // rayhit->its = std::make_shared<Intersection>(intersection);     
        
        rayhit->hit.geomID = shape->geometryID;
        rayhit->hit.primID = primID;
        rayhit->hit.u = u;  //重心坐标u
        rayhit->hit.v = v;  //重心坐标v


        //以下应该传入的是geometry normal，不是surface normal

        // rayhit->hit.Ng_x = intersection.normal[0];
        // rayhit->hit.Ng_y = intersection.normal[1];
        // rayhit->hit.Ng_z = intersection.normal[2];
    }
}

void UserShapeOcclude(const RTCOccludedFunctionNArguments *args) {
  // TODO 暂不实现
}


//获取一般物体embree描述（三角形mesh重新定义）
//https://github.com/embree/embree#user-geometry
RTCGeometry Shape::getEmbreeGeometry(RTCDevice device) const{
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);  //定义为user自定义类型
    rtcSetGeometryUserPrimitiveCount(geometry, 1);      //sets the number of primitives of a user geometry
    rtcSetGeometryUserData(geometry, (void *)this);     //sets user-defined data
    
    //获取embree中对自己渲染器的shape的描述
    rtcSetGeometryBoundsFunction(geometry, UserShapeBound, nullptr);    //记录boundingbox
    rtcSetGeometryIntersectFunction(geometry, UserShapeIntersect);      //记录求交用的函数
    rtcSetGeometryOccludedFunction(geometry, UserShapeOcclude);         //记录遮蔽用的函数（TODO）

    rtcCommitGeometry(geometry);
    return geometry;
}
