#include"Mesh.h"
#include<map>       //unordered mapҲ��

#define TINYOBJLOADER_IMPLEMENTATION    //��������ˣ�
#include"tinyobjloader/tiny_obj_loader.h"

using namespace tinyobj;

// �������������tiny obj loader��ʾ���÷�
// https://github.com/tinyobjloader/tinyobjloader/blob/release/README.md

std::shared_ptr<MeshData> MeshData::loadFromFile(std::string filepath){
    //�����ͬһ��mesh���ض�Σ�ģ�͸���ʱ��
    static std::map<std::string, std::shared_ptr<MeshData>> meshPool{};

    if(meshPool.count(filepath) != 0)
        return meshPool[filepath];      //ֱ�ӷ����Ѿ����غõ�ģ��

    ObjReader reader;           //���ļ�����һ�ַ�������vulkan�̳̼���ģ����һ��
    ObjReaderConfig config;     //��ȡ����
    config.triangulate = true;  //ȫ�������λ�
    // config.mtl_search_path = "./";  //material files��·��

    if(!reader.ParseFromFile(filepath, config)){    //��ȡ�������������Ϣ
        if(!reader.Error().empty()){
            std::cerr << "Error in parsing obj file " << filepath << " : " << reader.Error() << std::endl;
            exit(1);
        }
    }

    if(!reader.Warning().empty()){
        std::cerr << "Warning in parsing obj file " << filepath << " : " << reader.Warning() << std::endl;
    }

    //��ȡ�ɹ�����ʼд�����ݣ�
    const auto &attrib = reader.GetAttrib();    //����vertices, normals, texcoords
    const auto &shapes = reader.GetShapes();    //����separate objects & their faces, һ��face������Ӧ�Ķ��㣬�����а�����attrib�е�indices
    // if(shapes.size() != 1){
    //     std::cerr << "only support one mesh in each .obj file!\n";
    //     exit(1);
    // }

    std::shared_ptr<MeshData> mesh = std::make_shared<MeshData>();
    // const auto shape = shapes[0];   //��Ϊֻ�ܼ���һ��mesh(һ������)

    size_t vertexCount = attrib.vertices.size() / 3;
    std::vector<size_t> faceCounts;     //ÿ��shape��Ӧ��face������vector
    faceCounts.reserve(shapes.size());  

    size_t totalFaceCount = 0;
    for (int i = 0; i < shapes.size(); ++i){
        faceCounts.emplace_back(shapes[i].mesh.num_face_vertices.size());
        totalFaceCount += shapes[i].mesh.num_face_vertices.size();
    }

    mesh->faceCount = totalFaceCount;
    mesh->vertexCount = vertexCount;
    mesh->faceBuffer.reserve(totalFaceCount);    //reserve���������ݴ�����ʱ������
    mesh->vertexBuffer.reserve(vertexCount);

    //���faceBuffer
    for (int j = 0; j < shapes.size(); ++j) {
        for (int i = 0; i < faceCounts[j]; ++i){
            std::array<DataIndex, 3> triangleInfo;
            for (int v = 0; v < 3; ++v){
                DataIndex dataIndex;
                auto index = shapes[j].mesh.indices[i * 3 + v];
                dataIndex.vertexIndex = index.vertex_index;
                dataIndex.normalIndex = index.normal_index;
                dataIndex.texcoordIndex = index.texcoord_index;
                triangleInfo[v] = dataIndex;
            }
            mesh->faceBuffer.emplace_back(triangleInfo);
        }
    }

    const auto &vertexBuffer = attrib.vertices;
    const auto &normalBuffer = attrib.normals;
    const auto &texcoordBuffer = attrib.texcoords;

    //���vertexBuffer
    for (int i = 0; i < vertexCount; ++i){
        float vx = vertexBuffer[i * 3], vy = vertexBuffer[i * 3 + 1], vz = vertexBuffer[i * 3 + 2];
        mesh->vertexBuffer.emplace_back(Point3f(vx, vy, vz));
    }

    //�����Ƿ���normalbuffer��texcoordbuffer�������Ƿ����
    if(normalBuffer.size() != 0){
        int normNum = normalBuffer.size() / 3;
        mesh->normalBuffer.reserve(normNum);
        for (int i = 0; i < normNum; ++i){
            float nx = normalBuffer[i * 3], ny = normalBuffer[i * 3 + 1], nz = normalBuffer[i * 3 + 2];
            mesh->normalBuffer.emplace_back(Vector3f(nx, ny, nz));
        }
    }
    if (texcoordBuffer.size() != 0) {
        int texcodBufferSize = texcoordBuffer.size() / 2;
        mesh->texcoordBuffer.reserve(texcodBufferSize);
        for (int i = 0; i < texcodBufferSize; ++i) {
            float u = texcoordBuffer[i * 2], v = texcoordBuffer[i * 2 + 1];
            mesh->texcoordBuffer.emplace_back(Vector2f(u, v));
        }
    }

    return mesh;
}
