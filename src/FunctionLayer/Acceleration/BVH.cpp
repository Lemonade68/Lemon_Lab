#include"BVH.h"

//建造BVH的工具函数
void BVH::buildBVH(int l, int r, BVHNode *&root){
    if(l > r)       //左边界大于右边界则退出
        return;

    root = new BVHNode;

    //生成当前node的BVH
    for (int i = l; i <= r; ++i)
        root->box.Expand(shapeList[i]->getAABB());

    //如果不大于最大的三角形个数，返回叶子结点
    if((r - l + 1) <= bvhLeafMaxSize){
        root->ShapeNum = r - l + 1;
        root->firstShapeOffset = l;
        return;
    }

    //否则递归建树
    float lenx = root->box.pMax[0] - root->box.pMin[0];     //包围盒x轴长度
    float leny = root->box.pMax[1] - root->box.pMin[1];     //包围盒y轴长度
    float lenz = root->box.pMax[2] - root->box.pMin[2];     //包围盒z轴长度

    //按照最长的轴来进行划分（或者随机选轴进行划分也行）
    if(lenx >= leny && lenx >= lenz)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpx);
    //按y划分
    if(leny >= lenx && leny >= lenz)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpy);
    //按z划分
    if(lenz >= leny && lenz >= lenx)
        std::sort(shapeList.begin() + l, shapeList.begin() + r + 1, cmpz);

    //递归
    int mid = (l + r) / 2;                  //l和r相差为1时，mid = l，从而下面就是ll以及rr，各构建一个物体的bounding box
    buildBVH(l, mid, root->left);
    buildBVH(mid + 1, r, root->right);      //这里+1，防止与上面一个包含重复的物体
}

//求交BVH与光线是否相交的工具函数(注意要实际改变ray)
bool BVH::rayIntersectBVH(BVHNode *root, Ray &ray, Intersection &intersection) const{
    //如果是叶子结点：遍历其中所有的物体，使用其与光线求交的方法，来返回求交值，以及intersection值

    if(root->ShapeNum != 0){
        bool hit_least_one = false;     //是否有物体与光线相交
        for (int i = root->firstShapeOffset; i <= root->firstShapeOffset + root->ShapeNum - 1; ++i){
            bool hit = shapeList[i]->rayIntersectShape(ray, intersection);      //这里intersection会自动更新
            if(hit)
                hit_least_one = true;
        }
        if(hit_least_one)
            return true;
        else
            return false;
    }
    //如果是非叶子结点
    else{
        //如果光线与包围盒没有交点，则直接返回false；有则继续
        if(root->box.RayIntersect(ray)){
            bool result1 = rayIntersectBVH(root->left, ray, intersection),
                 result2 = rayIntersectBVH(root->right, ray, intersection);
            return (result1 || result2);
        }
        return false;
    }
}

//这里有过root未生成成功
void BVH::build(){
    buildBVH(0, shapeList.size() - 1, root);
}

bool BVH::rayIntersect(Ray &ray, Intersection &intersection) const{
    return rayIntersectBVH(root, ray, intersection);
}