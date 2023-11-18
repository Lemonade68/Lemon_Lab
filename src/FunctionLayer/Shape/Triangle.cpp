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

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);
    
    //设置三角形包围盒
    boundingBox.Expand(vtx0);
    boundingBox.Expand(vtx1);
    boundingBox.Expand(vtx2);
}

bool Triangle::rayIntersectShape(Ray &ray, Intersection &intersection) const{
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
    // hitpoint = vtx0 + u * e0 + v * e1, 0 <= u, v <= 1
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
        intersection.position = hitpoint;
        intersection.t = t;
        intersection.shape = this;
        //插值计算法线与纹理坐标
        float u = u_, v = v_, w = 1.f - u - v;
        auto faceInfo = meshData->faceBuffer[primID];

        //若规定了法线（平面上各点法线用插值进行计算）
        if(meshData->normalBuffer.size() != 0){
            Vector3f nw = transform.toWorld(meshData->normalBuffer[faceInfo[0].normalIndex]),
                     nu = transform.toWorld(meshData->normalBuffer[faceInfo[1].normalIndex]),
                     nv = transform.toWorld(meshData->normalBuffer[faceInfo[2].normalIndex]);
            intersection.normal = normalize(w * nw + u * nu + v * nv);
        } else {
            intersection.normal = paralNormal;      //未规定则直接认为是平面的法线
        }

        //若规定了纹理坐标
        if (meshData->texcoordBuffer.size() != 0) {
            Vector2f tw = meshData->texcoordBuffer[faceInfo[0].texcoordIndex],
                    tu = meshData->texcoordBuffer[faceInfo[1].texcoordIndex],
                    tv = meshData->texcoordBuffer[faceInfo[2].texcoordIndex];
            intersection.texCoord = w * tw + u * tu + v * tv;
        } else {
            intersection.texCoord = Vector2f{.0f, .0f};
        }

        //计算交点切线与副切线
        Vector3f tangent{1.f, 0.f, .0f};
        Vector3f bitangent;
        if (std::abs(dot(tangent, intersection.normal)) > .9f) {
            tangent = Vector3f(.0f, 1.f, .0f);
        }
        bitangent = normalize(cross(tangent, intersection.normal));
        tangent = normalize(cross(intersection.normal, bitangent));
        intersection.tangent = tangent;
        intersection.bitangent = bitangent;
        
        return true;
    }

    return false;
}

void TriangleMesh::initInternalAcceleration() {
    innerAcceleration = std::make_shared<BVH>(maxLeafSize);     //内部加速结构为叶子结点最大数为maxLeafSize的bvh
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

