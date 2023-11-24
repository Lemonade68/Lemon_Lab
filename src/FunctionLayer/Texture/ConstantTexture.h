#pragma once

#include"Texture.h"
#include"CoreLayer/ColorSpace/Spectrum.h"

//ע������̳�ʱ����Ҳ��Ҫд��templateֵ
template <typename TReturn> class ConstantTexture : public Texture<TReturn>{
public:
    //����ʹ����ģ�壬�����ʹ��ʱҲ��ҪĬ��ָ��value��ֵ
    ConstantTexture(const TReturn &_value) : value(_value) {}

    //Constant textureֱ�ӷ��س���ֵ����
    virtual TReturn evaluate(const TextureCoord &texCoord, const Point3f &position) const override{
        return value;
    }

    virtual TReturn evaluate(const Intersection &intersection) const override{
        return value;
    }

private:
    TReturn value;
};