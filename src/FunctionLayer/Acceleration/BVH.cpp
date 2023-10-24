#include"BVH.h"

//����BVH�Ĺ��ߺ���
void BVH::buildBVH(int l, int r, BVHNode *&root){
    if(l > r)       //��߽�����ұ߽����˳�
        return;

    root = new BVHNode;

    //���ɵ�ǰnode��BVH
    for (int i = l; i <= r; ++i)
        root->box.Expand(shapeList[i]->getAABB());

    //������������������θ���������Ҷ�ӽ��
    if((r - l + 1) <= bvhLeafMaxSize){
        root->ShapeNum = r - l + 1;
        root->firstShapeOffset = l;
        return;
    }

    //����ݹ齨��
    float lenx = root->box.pMax[0] - root->box.pMin[0];     //��Χ��x�᳤��
    float leny = root->box.pMax[1] - root->box.pMin[1];     //��Χ��y�᳤��
    float lenz = root->box.pMax[2] - root->box.pMin[2];     //��Χ��z�᳤��

    //��������������л��֣��������ѡ����л���Ҳ�У�
    if(lenx >= leny && lenx >= lenz)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpx);
    //��y����
    if(leny >= lenx && leny >= lenz)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpy);
    //��z����
    if(lenz >= leny && lenz >= lenx)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpz);

    //�ݹ�
    int mid = (l + r) / 2;                  //l��r���Ϊ1ʱ��mid = l���Ӷ��������ll�Լ�rr��������һ�������bounding box
    buildBVH(l, mid, root->left);
    buildBVH(mid + 1, r, root->right);      //����+1����ֹ������һ�������ظ�������
}

//��BVH������Ƿ��ཻ�Ĺ��ߺ���(ע��Ҫʵ�ʸı�ray)
bool BVH::rayIntersectBVH(BVHNode *root, Ray &ray, Intersection &intersection) const{
    //�����Ҷ�ӽ�㣺�����������е����壬ʹ����������󽻵ķ�������������ֵ���Լ�intersectionֵ

    if(root->ShapeNum != 0){
        bool hit_least_one = false;     //�Ƿ�������������ཻ
        for (int i = root->firstShapeOffset; i <= root->firstShapeOffset + root->ShapeNum - 1; ++i){
            bool hit = shapeList[i]->rayIntersectShape(ray, intersection);      //����intersection���Զ�����
            if(hit)
                hit_least_one = true;
        }
        if(hit_least_one)
            return true;
        else
            return false;
    }
    //����Ƿ�Ҷ�ӽ��
    else{
        //����������Χ��û�н��㣬��ֱ�ӷ���false���������
        if(root->box.RayIntersect(ray)){
            bool result1 = rayIntersectBVH(root->left, ray, intersection),
                 result2 = rayIntersectBVH(root->right, ray, intersection);
            return (result1 || result2);
        }
        return false;
    }
}

//�����й�rootδ���ɳɹ�
void BVH::build(){
    buildBVH(0, shapeList.size() - 1, root);
}

bool BVH::rayIntersect(Ray &ray, Intersection &intersection) const{
    return rayIntersectBVH(root, ray, intersection);
}