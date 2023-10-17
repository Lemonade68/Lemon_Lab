#pragma once

#include"Texture.h"
#include"CoreLayer/ColorSpace/Spectrum.h"

//暂时直接使用简单的unsigned char来存储信息，不添加image类，后面再重构
//图片纹理的话肯定用的是spectrum来得到信息，直接指定TReturn的值
class ImageTexture : public Texture<Spectrum>{
public:
    ImageTexture() = default;
    ImageTexture(unsigned char *_data, int nx, int ny){
        data = _data;
        size = Vector2i(nx, ny);
    }

    ~ImageTexture() { delete data; }        //析构时记得删除指针对应内存

    virtual Spectrum evaluate(const TextureCoord &texCoord) const override;

    virtual Spectrum evaluate(const Intersection &intersection) const override;

private:
    //大小已经在Texture类中定义
    unsigned char *data;        //unsigned char存储颜色：8bit对应256位的RGb
};