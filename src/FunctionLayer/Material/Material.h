#pragma once

#include"BxDF/BSDF.h"

class Material{
public:
    Material(){}

    //ע�⣺���ص�BSDF�Ѿ������cos��(��)
    virtual std::shared_ptr<BSDF> computeBSDF(const Intersection &intersection) const = 0;

    // TODO��������������⣨������ͼ��

protected:
    // ������ͼ
    // std::shared_ptr<NormalTexture> normalMap;
};