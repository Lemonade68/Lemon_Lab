#pragma once

#include"CoreLayer/Math/Math.h"
#include<memory>

// moer-lite�е�˵����
//* Image��������֯��ʽ����
//* [0, 0] ----X-----
//* |               |
//* |               |
//* Y        [x, y] |
//* -----------------
//* ����[0, 0]��ӦͼƬ���Ͻ�
//* ����[x, y]��ӦͼƬ��x�У���y��

//����png����hdrʱʹ�õ�ͼ����
class Image{
public:
    Image() = delete;

    Image(Vector2i _size) : size(_size) {
        data = new float[_size[0] * _size[1] * channels];
    }

    Image(Vector2i _size, float *_data) : size(_size), data(_data) {}

    ~Image() { delete[] data; }

    Vector3f getValue(const Vector2i &xy) const;    //��ȡ����[x,y]����ͨ��ֵ�������ͼƬһ��ʹ��

    void setValue(const Vector2i &xy, const Vector3f &val);     //����[x,y]������ͨ��ֵ

    //��HDR��ʽ����ͼƬ
    void saveHDR(const char *filename) const;

    //TODO����PNG��ʽ����ͼƬ
    void savePNG(const char *filename) const;

public:
    Vector2i size;
    static constexpr int channels = 3;      //RGB��ͨ��

private:
    float *data = nullptr;      //ָ��float�������ݵ�����
};