#pragma once

#include"Texture.h"
#include"CoreLayer/ColorSpace/Spectrum.h"

//注意子类继承时父类也需要写上template值
template <typename TReturn> class ConstantTexture : public Texture<TReturn>{
public:
    //由于使用了模板，因此在使用时也需要默认指定value的值
    ConstantTexture(const TReturn &_value) : value(_value) {}

    //Constant texture直接返回常量值即可
    virtual TReturn evaluate(const TextureCoord &texCoord, const Point3f &position) const override{
        return value;
    }

    virtual TReturn evaluate(const Intersection &intersection) const override{
        return value;
    }

private:
    TReturn value;
};