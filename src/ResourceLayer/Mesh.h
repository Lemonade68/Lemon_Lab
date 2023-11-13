#pragma once
#include"CoreLayer/Math/Math.h"
#include<array>
#include<memory>
#include<vector>



//Ĭ��Ϊ��������Ԫ

struct DataIndex{
    int vertexIndex;    //��������
    int normalIndex;    //��������
    int texcoordIndex;  //������������
};

struct MeshData{
    int faceCount;      //�����θ���
    int vertexCount;    //�������
    std::vector<Point3f> vertexBuffer;      //mesh�����ж���
    std::vector<Vector3f> normalBuffer;     //mesh�����з���
    std::vector<Vector2f> texcoordBuffer;   //����������������
    std::vector<std::array<DataIndex, 3>> faceBuffer;   //ÿ�������������������Ӧ�����ݵ�����

    //����obj�ļ���MeshData�У�ֻ֧�ּ��ص�һ��Mesh��
    static std::shared_ptr<MeshData> loadFromFile(std::string filepath);
};


