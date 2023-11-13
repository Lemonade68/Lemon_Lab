#pragma once
#include"CoreLayer/Math/Math.h"
#include<array>
#include<memory>
#include<vector>



//默认为三角形面元

struct DataIndex{
    int vertexIndex;    //顶点索引
    int normalIndex;    //法线索引
    int texcoordIndex;  //纹理坐标索引
};

struct MeshData{
    int faceCount;      //三角形个数
    int vertexCount;    //顶点个数
    std::vector<Point3f> vertexBuffer;      //mesh中所有顶点
    std::vector<Vector3f> normalBuffer;     //mesh中所有法线
    std::vector<Vector2f> texcoordBuffer;   //所有纹理坐标索引
    std::vector<std::array<DataIndex, 3>> faceBuffer;   //每个三角形面三个顶点对应的数据的索引

    //加载obj文件到MeshData中（只支持加载单一的Mesh）
    static std::shared_ptr<MeshData> loadFromFile(std::string filepath);
};


