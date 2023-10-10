#pragma once

#include"BxDF/BSDF.h"

class Material{
public:
    Material(){}

    //注意：返回的BSDF已经计算过cos项(？)
    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const = 0;

    // TODO：处理法线相关问题（法线贴图）

protected:
    // 法线贴图
    // std::shared_ptr<NormalTexture> normalMap;
};