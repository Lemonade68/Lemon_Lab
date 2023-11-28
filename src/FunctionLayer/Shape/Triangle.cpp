#include"Triangle.h"

//修改这个构造函数，使其能够使用triangle mesh的material的光源等
Triangle::Triangle(int _primID, int _vtx0Idx, int _vtx1Idx, int _vtx2Idx, const TriangleMesh * _mesh)
     : Shape(_mesh->material, _mesh->light){

    //初始化属性
    transform = _mesh->transform;   //令三角形transform = 整个模型的transform
    primID = _primID;
    vertex0Index = _vtx0Idx;
    vertex1Index = _vtx1Idx;
    vertex2Index = _vtx2Idx;
    meshData = _mesh->meshData;
    //设置三角形的geomID
    geometryID = _mesh->geometryID;

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);
    
    //设置三角形包围盒
    boundingBox.Expand(vtx0);
    boundingBox.Expand(vtx1);
    boundingBox.Expand(vtx2);
}


//算法来源：Moer-lite      u,v为三角形的重心坐标中的两个数
bool Triangle::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const {
    // std::cerr << "1";        //使用embree时不会使用该函数（TriangleMesh中的geometry是内置三角形类型，不是user类型）

    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);

    Vector3f edge0 = vtx1 - vtx0, edge1 = vtx2 - vtx0;
    Vector3f paralNormal = normalize(cross(edge0, edge1));  //平面法线

    //与平面求交
    float d = -dot(paralNormal, Vector3f{vtx0[0], vtx0[1], vtx0[2]});
    float a = dot(paralNormal, Vector3f{origin[0], origin[1], origin[2]}) + d;
    float b = dot(paralNormal, direction);
    if (b == .0f)
        return false; // miss
    float t = -a / b;

    if (t < ray.tNear || t > ray.tFar)
        return false;

    Point3f hitpoint = origin + t * direction;
    Vector3f v1 = cross(hitpoint - vtx0, edge1), v2 = cross(edge0, edge1);
    float u_ = v1.length() / v2.length();
    if (dot(v1, v2) < 0)
        u_ *= -1;

    v1 = cross(hitpoint - vtx0, edge0);
    v2 = cross(edge1, edge0);
    float v_ = v1.length() / v2.length();
    if (dot(v1, v2) < 0)
        v_ *= -1;

    if (u_ >= .0f && v_ >= .0f && (u_ + v_ <= 1.f)) {
        ray.tFar = t;
        *primID = this->primID;
        *u = u_;
        *v = v_;
        return true;
    }

    return false;
}


//实际用不到这个函数，都是使用TriangleMesh的fillIntersection和rayIntersectShape
void Triangle::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const{
    return;
}


// //单个三角形与光线求交算法
// //见 https://github.com/embree/embree/blob/master/tutorials/minimal/minimal.cpp
// RTCGeometry Triangle::getEmbreeGeometry(RTCDevice device) const {
//     //由于Embree中自带三角形求交算法，因此这里不使用RTC_GEOMETRY_TYPE_USER
//     RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

//     float *vertices = (float *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX,
//                                                            0, RTC_FORMAT_FLOAT3, 3 * sizeof(float),
//                                                            3);
//     for (int i = 0; i < 3; ++i){
//         Point3f vertex = transform.toWorld(meshData->vertexBuffer[i]);
//         vertices[3 * i] = vertex[0];
//         vertices[3 * i + 1] = vertex[1];
//         vertices[3 * i + 2] = vertex[2];     
//     }

//     unsigned *indices = (unsigned *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX,
//                                                                 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned),
//                                                                 1);
//     indices[0] = 0;
//     indices[1] = 1;
//     indices[2] = 2;

//     rtcCommitGeometry(geometry);
//     return geometry;
// }


//直接在内部的加速结构中进行查找
//实际上当使用embree的时候，会使用内部的求交方法，这个方法不会被调用
bool TriangleMesh::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const {
    int geomID = -1;
    return innerAcceleration->rayIntersect(ray, &geomID, primID, u, v);
}


//使用Embree时，接受获取的信息，填充到intersection中
void TriangleMesh::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const{
    intersection->t = tFar;
    intersection->shape = this;

    //在序号为primID的三角形内部用插值计算交点、法线以及纹理坐标
    auto faceInfo = meshData->faceBuffer[primID];
    float w = 1.f - u - v;      //w, u, v分别对应顶点0 1 2

    //计算position
    Point3f pw = transform.toWorld(meshData->vertexBuffer[faceInfo[0].vertexIndex]),    
            pu = transform.toWorld(meshData->vertexBuffer[faceInfo[1].vertexIndex]),
            pv = transform.toWorld(meshData->vertexBuffer[faceInfo[2].vertexIndex]);
    intersection->position = Point3f{
        w * pw[0] + u * pu[0] + v * pv[0],
        w * pw[1] + u * pu[1] + v * pv[1],
        w * pw[2] + u * pu[2] + v * pv[2]};

    //计算normal
    if (meshData->normalBuffer.size() != 0) {       //如果有手动记录法线，则设置为插值
        Vector3f nw = transform.toWorld(meshData->normalBuffer[faceInfo[0].normalIndex]),
                 nu = transform.toWorld(meshData->normalBuffer[faceInfo[1].normalIndex]),
                 nv = transform.toWorld(meshData->normalBuffer[faceInfo[2].normalIndex]);
        intersection->normal = normalize(w * nw + u * nu + v * nv);
    } else {
        intersection->normal = normalize(cross(pu - pw, pv - pw)); // 不然直接设置为三角形面的normal
    }

    //计算纹理坐标
    if (meshData->texcoordBuffer.size() != 0) {
        Vector2f tw = meshData->texcoordBuffer[faceInfo[0].texcoordIndex],
                tu = meshData->texcoordBuffer[faceInfo[1].texcoordIndex],
                tv = meshData->texcoordBuffer[faceInfo[2].texcoordIndex];
        intersection->texCoord = w * tw + u * tu + v * tv;
    } else {
        intersection->texCoord = Vector2f{.0f, .0f};
    }

    //计算(bi)tangent
    Vector3f tangent{1.f, 0.f, .0f};
    Vector3f bitangent;
    if (std::abs(dot(tangent, intersection->normal)) > .9f) {
        tangent = Vector3f(.0f, 1.f, .0f);
    }
    bitangent = normalize(cross(tangent, intersection->normal));
    tangent = normalize(cross(intersection->normal, bitangent));
    intersection->tangent = tangent;
    intersection->bitangent = bitangent;

}


//这里暂时手动切换使用BVH还是EmbreeBVH
//当使用Embree时，这个函数实际上没啥作用，因为整个模型是被当做内置的三角形类型进行求交的
//这里的作用主要是给BVH与其他加速结构使用（哪怕外部使用的是embree，这里写成bvh也没问题）
void TriangleMesh::initInternalAcceleration() {
    // innerAcceleration = std::make_shared<BVH>(maxLeafSize);     //内部加速结构为叶子结点最大数为maxLeafSize的bvh
    innerAcceleration = std::make_shared<EmbreeBVH>();
    
    innerAcceleration->shapeList.reserve(meshData->faceCount);      //提前预留好位置
    int primCount = meshData->faceCount;
    for (int primID = 0; primID < primCount; ++primID){
        int vtx0Idx = meshData->faceBuffer[primID][0].vertexIndex;
        int vtx1Idx = meshData->faceBuffer[primID][1].vertexIndex;
        int vtx2Idx = meshData->faceBuffer[primID][2].vertexIndex;
        std::shared_ptr<Triangle> triangle =
            std::make_shared<Triangle>(primID, vtx0Idx, vtx1Idx, vtx2Idx, this);
        innerAcceleration->attchShape(triangle);
    }
    innerAcceleration->build();     //构建内部加速结构
    boundingBox = innerAcceleration->boundingBox;       //整体的bounding box
}


//在使用TriangleMesh的embree时，直接使用embree内置的求交算法，不会使用自己的求交函数（即Triangle和TriangleMesh的求交方法都不会用）
RTCGeometry TriangleMesh::getEmbreeGeometry(RTCDevice device) const {
    //由于Embree中自带三角形求交算法，因此这里不使用RTC_GEOMETRY_TYPE_USER
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    float *vertexBuffer = (float *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX,
                                                           0, RTC_FORMAT_FLOAT3, 3 * sizeof(float),
                                                           meshData->vertexCount);
    //vertexBuffer是xyz坐标分开记录的
    for (int i = 0; i < meshData->vertexCount; ++i){
        Point3f vertex = transform.toWorld(meshData->vertexBuffer[i]);
        vertexBuffer[3 * i] = vertex[0];        //x
        vertexBuffer[3 * i + 1] = vertex[1];    //y
        vertexBuffer[3 * i + 2] = vertex[2];    //z
    }

    unsigned *indexBuffer = (unsigned *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX,
                                                                0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned),
                                                                meshData->faceCount);
    //indexBuffer是逐顶点记录的
    for (int i = 0; i < meshData->faceCount; ++i) {
        indexBuffer[i * 3] = meshData->faceBuffer[i][0].vertexIndex;
        indexBuffer[i * 3 + 1] = meshData->faceBuffer[i][1].vertexIndex;
        indexBuffer[i * 3 + 2] = meshData->faceBuffer[i][2].vertexIndex;
    }

    rtcCommitGeometry(geometry);    //setting up geometries后必须commit
    return geometry;
}