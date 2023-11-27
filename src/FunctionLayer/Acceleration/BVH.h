#pragma once

#include"AABB.h"
#include"FunctionLayer/Shape/Shape.h"
#include"Acceleration.h"

struct BVHNode{
    BVHNode *left = nullptr, *right = nullptr;      //�����ӽڵ�(ָ��ǵó�ʼ��)
    AABB box;                       //��Χ��
    int ShapeNum = 0;               //��Χ���д��˶������壬Ϊ0��ʾ��Ҷ�ӽ�㣬��Ϊ0��ʾҶ�ӽ��
    int firstShapeOffset = 0;       //��Χ���е�һ�������ƫ����
};

//��ӡBVH��Ϣ(ͷ�ļ��мǵö���Ϊinline������)
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

    virtual bool rayIntersect(Ray &ray, int *geomID, int *primID, float *u, float *v) const override;    //������󽻵Ľӿ�

    virtual void debugPrint() const override{
        PrintBVHNode(root);
    }

private:
    int bvhLeafMaxSize = 1;         //bvh����������������������(������Ϊstatic�Ӷ����Գ�ʼ��ʱ�޸�)
    BVHNode *root = nullptr;        //���ڵ㣬����BVH����

    //����BVH�Ĺ��ߺ���
    void buildBVH(int l, int r, BVHNode *&root);
    bool rayIntersectBVH(BVHNode *root, Ray &ray, int *geomID, int *primID, float *u, float *v) const;

    //���հ�Χ�����ĵ������������ν�ʺ���
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
