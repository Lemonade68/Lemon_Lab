#pragma once

#include"AABB.h"
#include"FunctionLayer/Shape/Shape.h"
#include"Acceleration.h"

struct BVHNode{
    BVHNode *left = nullptr, *right = nullptr;      //左右子节点(指针记得初始化)
    AABB box;                       //包围盒
    int ShapeNum = 0;               //包围盒中存了多少物体，为0表示非叶子结点，不为0表示叶子结点
    int firstShapeOffset = 0;       //包围盒中第一个物体的偏移量
};

//打印BVH信息(头文件中记得定义为inline函数？)
inline void PrintBVHNode(BVHNode *root){
    printf("==========================\n");
    if(!root->ShapeNum)
        printf("Non-leaf node\n");
    else
        printf("leaf node\n");

    root->box.debugPrint();

    std::cout << "ShapeNum: " << root->ShapeNum << " " << "Offset: " << root->firstShapeOffset << "\n";

    printf("==========================\n");

    if(root->left){
        printf("\n--left node:\n");
        PrintBVHNode(root->left);
    }
    if(root->right){
        printf("\n--right node:\n");
        PrintBVHNode(root->right);
    }
}

class BVH : public Acceleration{
public:
    friend class Shape;

    BVH(int leafMaxSize = 1) : bvhLeafMaxSize(leafMaxSize) {}

    BVH(const std::vector<std::shared_ptr<Shape>> &_shapeList, int leafMaxSize = 1) 
        : Acceleration(_shapeList), bvhLeafMaxSize(leafMaxSize) {}

    virtual void build() override;

    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const override;    //与光线求交的接口

    virtual void debugPrint() const override{
        PrintBVHNode(root);
    }

private:
    int bvhLeafMaxSize = 1;         //bvh结点所包含的最大物体数量(不设置为static从而可以初始化时修改)
    BVHNode *root = nullptr;        //根节点，用于BVH索引

    //两个BVH的工具函数
    void buildBVH(int l, int r, BVHNode *&root);
    bool rayIntersectBVH(BVHNode *root, Ray &ray, int *geomID, int *primID, float *u, float *v) const;

    //按照包围盒中心的坐标来排序的谓词函数
    static bool cmpx(std::shared_ptr<Shape> t1, std::shared_ptr<Shape> t2) {
        return t1->boundingBox.Center()[0] < t2->boundingBox.Center()[0];
    }
    static bool cmpy(std::shared_ptr<Shape> t1, std::shared_ptr<Shape> t2) {
        return t1->boundingBox.Center()[1] < t2->boundingBox.Center()[1];
    }
    static bool cmpz(std::shared_ptr<Shape> t1, std::shared_ptr<Shape> t2) {
        return t1->boundingBox.Center()[2] < t2->boundingBox.Center()[2];
    }
};
