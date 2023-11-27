#include"Shape.h"
#include"embree3/rtcore_geometry.h"

//��ʼ������ʱ��Ҫ����transform��ʼ��
Shape::Shape(const std::shared_ptr<Material> &_material, const std::shared_ptr<Light> &_light, 
             const Vector3f &translate, const Vector3f &scale, const Vector3f &axis, float radian){
    //��ȡ��������
    Matrix4f tranlsateMat = Transform::translation(translate);
    Matrix4f scaleMat = Transform::scalation(scale);
    Matrix4f rotateMat = Transform::rotation(axis, radian);

    transform = Transform(tranlsateMat, rotateMat, scaleMat);

    material = _material;
    light = _light;
}

//TODO:�����������


// sets a callback to query the bounding box of user-defined primitives
void UserShapeBound(const RTCBoundsFunctionArguments *args) {
    Shape *shape = static_cast<Shape *>(args->geometryUserPtr);
    auto [pMin, pMax] = shape->getAABB();       //c++17���ṹ��������
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
    // RTCRayHit *rayhit = static_cast<RTCRayHit *>(args->rayhit);  //��ʾת��������Ч
    RTCRayHit *rayhit = (RTCRayHit *)args->rayhit;      //��¼ray��hit����Ϣ
    // UserRayHit1 *rayhit = (UserRayHit1 *)(args->rayhit);    //���intersection�ṹ��RTCRayHit

    //����rayhit��ray����Ϣת��Ϊ�Լ�����Ĺ���
    Point3f origin{rayhit->ray.org_x, rayhit->ray.org_y, rayhit->ray.org_z};
    Vector3f direction{rayhit->ray.dir_x, rayhit->ray.dir_y, rayhit->ray.dir_z};
    Ray ray{origin, direction, 1e-4f, rayhit->ray.tfar};

    //��rayhit�е�hit��¼��Ϣ
    int primID;
    float u, v;
    bool hit = shape->rayIntersectShape(ray, &primID, &u, &v);
    if(hit) {   //�������
        rayhit->ray.tfar = ray.tFar;    //�����У�����rayhit�е�tfarΪ���к���ߵ�tfar
        // rayhit->its = std::make_shared<Intersection>(intersection);     
        
        rayhit->hit.geomID = shape->geometryID;
        rayhit->hit.primID = primID;
        rayhit->hit.u = u;  //��������u
        rayhit->hit.v = v;  //��������v


        //����Ӧ�ô������geometry normal������surface normal

        // rayhit->hit.Ng_x = intersection.normal[0];
        // rayhit->hit.Ng_y = intersection.normal[1];
        // rayhit->hit.Ng_z = intersection.normal[2];
    }
}

void UserShapeOcclude(const RTCOccludedFunctionNArguments *args) {
  // TODO �ݲ�ʵ��
}


//��ȡһ������embree������������mesh���¶��壩
//https://github.com/embree/embree#user-geometry
RTCGeometry Shape::getEmbreeGeometry(RTCDevice device) const{
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);  //����Ϊuser�Զ�������
    rtcSetGeometryUserPrimitiveCount(geometry, 1);      //sets the number of primitives of a user geometry
    rtcSetGeometryUserData(geometry, (void *)this);     //sets user-defined data
    
    //��ȡembree�ж��Լ���Ⱦ����shape������
    rtcSetGeometryBoundsFunction(geometry, UserShapeBound, nullptr);    //��¼boundingbox
    rtcSetGeometryIntersectFunction(geometry, UserShapeIntersect);      //��¼���õĺ���
    rtcSetGeometryOccludedFunction(geometry, UserShapeOcclude);         //��¼�ڱ��õĺ�����TODO��

    rtcCommitGeometry(geometry);
    return geometry;
}
