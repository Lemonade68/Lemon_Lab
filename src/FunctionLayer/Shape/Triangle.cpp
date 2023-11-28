#include"Triangle.h"

//�޸�������캯����ʹ���ܹ�ʹ��triangle mesh��material�Ĺ�Դ��
Triangle::Triangle(int _primID, int _vtx0Idx, int _vtx1Idx, int _vtx2Idx, const TriangleMesh * _mesh)
     : Shape(_mesh->material, _mesh->light){

    //��ʼ������
    transform = _mesh->transform;   //��������transform = ����ģ�͵�transform
    primID = _primID;
    vertex0Index = _vtx0Idx;
    vertex1Index = _vtx1Idx;
    vertex2Index = _vtx2Idx;
    meshData = _mesh->meshData;
    //���������ε�geomID
    geometryID = _mesh->geometryID;

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);
    
    //���������ΰ�Χ��
    boundingBox.Expand(vtx0);
    boundingBox.Expand(vtx1);
    boundingBox.Expand(vtx2);
}


//�㷨��Դ��Moer-lite      u,vΪ�����ε����������е�������
bool Triangle::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const {
    // std::cerr << "1";        //ʹ��embreeʱ����ʹ�øú�����TriangleMesh�е�geometry���������������ͣ�����user���ͣ�

    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);

    Vector3f edge0 = vtx1 - vtx0, edge1 = vtx2 - vtx0;
    Vector3f paralNormal = normalize(cross(edge0, edge1));  //ƽ�淨��

    //��ƽ����
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


//ʵ���ò����������������ʹ��TriangleMesh��fillIntersection��rayIntersectShape
void Triangle::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const{
    return;
}


// //������������������㷨
// //�� https://github.com/embree/embree/blob/master/tutorials/minimal/minimal.cpp
// RTCGeometry Triangle::getEmbreeGeometry(RTCDevice device) const {
//     //����Embree���Դ����������㷨��������ﲻʹ��RTC_GEOMETRY_TYPE_USER
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


//ֱ�����ڲ��ļ��ٽṹ�н��в���
//ʵ���ϵ�ʹ��embree��ʱ�򣬻�ʹ���ڲ����󽻷���������������ᱻ����
bool TriangleMesh::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const {
    int geomID = -1;
    return innerAcceleration->rayIntersect(ray, &geomID, primID, u, v);
}


//ʹ��Embreeʱ�����ܻ�ȡ����Ϣ����䵽intersection��
void TriangleMesh::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const{
    intersection->t = tFar;
    intersection->shape = this;

    //�����ΪprimID���������ڲ��ò�ֵ���㽻�㡢�����Լ���������
    auto faceInfo = meshData->faceBuffer[primID];
    float w = 1.f - u - v;      //w, u, v�ֱ��Ӧ����0 1 2

    //����position
    Point3f pw = transform.toWorld(meshData->vertexBuffer[faceInfo[0].vertexIndex]),    
            pu = transform.toWorld(meshData->vertexBuffer[faceInfo[1].vertexIndex]),
            pv = transform.toWorld(meshData->vertexBuffer[faceInfo[2].vertexIndex]);
    intersection->position = Point3f{
        w * pw[0] + u * pu[0] + v * pv[0],
        w * pw[1] + u * pu[1] + v * pv[1],
        w * pw[2] + u * pu[2] + v * pv[2]};

    //����normal
    if (meshData->normalBuffer.size() != 0) {       //������ֶ���¼���ߣ�������Ϊ��ֵ
        Vector3f nw = transform.toWorld(meshData->normalBuffer[faceInfo[0].normalIndex]),
                 nu = transform.toWorld(meshData->normalBuffer[faceInfo[1].normalIndex]),
                 nv = transform.toWorld(meshData->normalBuffer[faceInfo[2].normalIndex]);
        intersection->normal = normalize(w * nw + u * nu + v * nv);
    } else {
        intersection->normal = normalize(cross(pu - pw, pv - pw)); // ��Ȼֱ������Ϊ���������normal
    }

    //������������
    if (meshData->texcoordBuffer.size() != 0) {
        Vector2f tw = meshData->texcoordBuffer[faceInfo[0].texcoordIndex],
                tu = meshData->texcoordBuffer[faceInfo[1].texcoordIndex],
                tv = meshData->texcoordBuffer[faceInfo[2].texcoordIndex];
        intersection->texCoord = w * tw + u * tu + v * tv;
    } else {
        intersection->texCoord = Vector2f{.0f, .0f};
    }

    //����(bi)tangent
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


//������ʱ�ֶ��л�ʹ��BVH����EmbreeBVH
//��ʹ��Embreeʱ���������ʵ����ûɶ���ã���Ϊ����ģ���Ǳ��������õ����������ͽ����󽻵�
//�����������Ҫ�Ǹ�BVH���������ٽṹʹ�ã������ⲿʹ�õ���embree������д��bvhҲû���⣩
void TriangleMesh::initInternalAcceleration() {
    // innerAcceleration = std::make_shared<BVH>(maxLeafSize);     //�ڲ����ٽṹΪҶ�ӽ�������ΪmaxLeafSize��bvh
    innerAcceleration = std::make_shared<EmbreeBVH>();
    
    innerAcceleration->shapeList.reserve(meshData->faceCount);      //��ǰԤ����λ��
    int primCount = meshData->faceCount;
    for (int primID = 0; primID < primCount; ++primID){
        int vtx0Idx = meshData->faceBuffer[primID][0].vertexIndex;
        int vtx1Idx = meshData->faceBuffer[primID][1].vertexIndex;
        int vtx2Idx = meshData->faceBuffer[primID][2].vertexIndex;
        std::shared_ptr<Triangle> triangle =
            std::make_shared<Triangle>(primID, vtx0Idx, vtx1Idx, vtx2Idx, this);
        innerAcceleration->attchShape(triangle);
    }
    innerAcceleration->build();     //�����ڲ����ٽṹ
    boundingBox = innerAcceleration->boundingBox;       //�����bounding box
}


//��ʹ��TriangleMesh��embreeʱ��ֱ��ʹ��embree���õ����㷨������ʹ���Լ����󽻺�������Triangle��TriangleMesh���󽻷����������ã�
RTCGeometry TriangleMesh::getEmbreeGeometry(RTCDevice device) const {
    //����Embree���Դ����������㷨��������ﲻʹ��RTC_GEOMETRY_TYPE_USER
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    float *vertexBuffer = (float *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX,
                                                           0, RTC_FORMAT_FLOAT3, 3 * sizeof(float),
                                                           meshData->vertexCount);
    //vertexBuffer��xyz����ֿ���¼��
    for (int i = 0; i < meshData->vertexCount; ++i){
        Point3f vertex = transform.toWorld(meshData->vertexBuffer[i]);
        vertexBuffer[3 * i] = vertex[0];        //x
        vertexBuffer[3 * i + 1] = vertex[1];    //y
        vertexBuffer[3 * i + 2] = vertex[2];    //z
    }

    unsigned *indexBuffer = (unsigned *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX,
                                                                0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned),
                                                                meshData->faceCount);
    //indexBuffer���𶥵��¼��
    for (int i = 0; i < meshData->faceCount; ++i) {
        indexBuffer[i * 3] = meshData->faceBuffer[i][0].vertexIndex;
        indexBuffer[i * 3 + 1] = meshData->faceBuffer[i][1].vertexIndex;
        indexBuffer[i * 3 + 2] = meshData->faceBuffer[i][2].vertexIndex;
    }

    rtcCommitGeometry(geometry);    //setting up geometries�����commit
    return geometry;
}