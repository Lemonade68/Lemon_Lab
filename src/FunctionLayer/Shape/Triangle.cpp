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

    Point3f vtx0 = transform.toWorld(meshData->vertexBuffer[vertex0Index]),
            vtx1 = transform.toWorld(meshData->vertexBuffer[vertex1Index]),
            vtx2 = transform.toWorld(meshData->vertexBuffer[vertex2Index]);
    
    //���������ΰ�Χ��
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
        //��ֵ���㷨������������
        float u = u_, v = v_, w = 1.f - u - v;
        auto faceInfo = meshData->faceBuffer[primID];

        //���涨�˷��ߣ�ƽ���ϸ��㷨���ò�ֵ���м��㣩
        if(meshData->normalBuffer.size() != 0){
            Vector3f nw = transform.toWorld(meshData->normalBuffer[faceInfo[0].normalIndex]),
                     nu = transform.toWorld(meshData->normalBuffer[faceInfo[1].normalIndex]),
                     nv = transform.toWorld(meshData->normalBuffer[faceInfo[2].normalIndex]);
            intersection.normal = normalize(w * nw + u * nu + v * nv);
        } else {
            intersection.normal = paralNormal;      //δ�涨��ֱ����Ϊ��ƽ��ķ���
        }

        //���涨����������
        if (meshData->texcoordBuffer.size() != 0) {
            Vector2f tw = meshData->texcoordBuffer[faceInfo[0].texcoordIndex],
                    tu = meshData->texcoordBuffer[faceInfo[1].texcoordIndex],
                    tv = meshData->texcoordBuffer[faceInfo[2].texcoordIndex];
            intersection.texCoord = w * tw + u * tu + v * tv;
        } else {
            intersection.texCoord = Vector2f{.0f, .0f};
        }

        //���㽻�������븱����
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
    innerAcceleration = std::make_shared<BVH>(maxLeafSize);     //�ڲ����ٽṹΪҶ�ӽ�������ΪmaxLeafSize��bvh
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

