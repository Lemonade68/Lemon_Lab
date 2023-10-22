#pragma once

#include"CoreLayer/Math/Math.h"
#include<memory>

// moer-lite中的说明：
//* Image的数据组织格式如下
//* [0, 0] ----X-----
//* |               |
//* |               |
//* Y        [x, y] |
//* -----------------
//* 坐标[0, 0]对应图片左上角
//* 坐标[x, y]对应图片第x列，第y行

//生成png或是hdr时使用的图像类
class Image{
public:
    Image() = delete;

    Image(Vector2i _size) : size(_size) {
        data = new float[_size[0] * _size[1] * channels];
    }

    Image(Vector2i _size, float *_data) : size(_size), data(_data) {}

    ~Image() { delete[] data; }

    Vector3f getValue(const Vector2i &xy) const;    //获取坐标[x,y]的三通道值，与加载图片一起使用

    void setValue(const Vector2i &xy, const Vector3f &val);     //设置[x,y]处的三通道值

    //以HDR形式保存图片
    void saveHDR(const char *filename) const;

    //TODO：以PNG形式保存图片
    void savePNG(const char *filename) const;

public:
    Vector2i size;
    static constexpr int channels = 3;      //RGB三通道

private:
    float *data = nullptr;      //指向float类型数据的数组
};