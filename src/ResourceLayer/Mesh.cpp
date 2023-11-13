#include"Mesh.h"
#include<map>       //unordered map也行

#define TINYOBJLOADER_IMPLEMENTATION    //这个别忘了！
#include"tinyobjloader/tiny_obj_loader.h"

using namespace tinyobj;

// 下面的链接中有tiny obj loader的示例用法
// https://github.com/tinyobjloader/tinyobjloader/blob/release/README.md

std::shared_ptr<MeshData> MeshData::loadFromFile(std::string filepath){
    //避免对同一个mesh加载多次（模型复用时）
    static std::map<std::string, std::shared_ptr<MeshData>> meshPool{};

    if(meshPool.count(filepath) != 0)
        return meshPool[filepath];      //直接返回已经加载好的模型

    ObjReader reader;           //读文件的另一种方法：见vulkan教程加载模型那一章
    ObjReaderConfig config;     //读取配置
    config.triangulate = true;  //全部三角形化
    // config.mtl_search_path = "./";  //material files的路径

    if(!reader.ParseFromFile(filepath, config)){    //读取错误，输出错误信息
        if(!reader.Error().empty()){
            std::cerr << "Error in parsing obj file " << filepath << " : " << reader.Error() << std::endl;
            exit(1);
        }
    }

    if(!reader.Warning().empty()){
        std::cerr << "Warning in parsing obj file " << filepath << " : " << reader.Warning() << std::endl;
    }

    //读取成功，开始写入数据：
    const auto &attrib = reader.GetAttrib();    //包含vertices, normals, texcoords
    const auto &shapes = reader.GetShapes();    //包含separate objects & their faces, 一个face包含对应的顶点，顶点中包含在attrib中的indices
    // if(shapes.size() != 1){
    //     std::cerr << "only support one mesh in each .obj file!\n";
    //     exit(1);
    // }

    std::shared_ptr<MeshData> mesh = std::make_shared<MeshData>();
    // const auto shape = shapes[0];   //因为只能加载一个mesh(一个物体)

    size_t vertexCount = attrib.vertices.size() / 3;
    std::vector<size_t> faceCounts;     //每个shape对应的face个数的vector
    faceCounts.reserve(shapes.size());  

    size_t totalFaceCount = 0;
    for (int i = 0; i < shapes.size(); ++i){
        faceCounts.emplace_back(shapes[i].mesh.num_face_vertices.size());
        totalFaceCount += shapes[i].mesh.num_face_vertices.size();
    }

    mesh->faceCount = totalFaceCount;
    mesh->vertexCount = vertexCount;
    mesh->faceBuffer.reserve(totalFaceCount);    //reserve来减少扩容带来的时间消耗
    mesh->vertexBuffer.reserve(vertexCount);

    //填充faceBuffer
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

    //填充vertexBuffer
    for (int i = 0; i < vertexCount; ++i){
        float vx = vertexBuffer[i * 3], vy = vertexBuffer[i * 3 + 1], vz = vertexBuffer[i * 3 + 2];
        mesh->vertexBuffer.emplace_back(Point3f(vx, vy, vz));
    }

    //根据是否有normalbuffer和texcoordbuffer来决定是否填充
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
