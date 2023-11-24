#include"ImageTexture.h"

//直接从纹理坐标获取图片纹理的信息
Spectrum ImageTexture::evaluate(const TextureCoord &texCoord, const Point3f &position) const{
    if(data == nullptr)
        return Spectrum(1.f, .0f, .0f);     //返回纯红色，表示图片加载错误

    auto u = texCoord.coord[0];        //float类型
    auto v = texCoord.coord[1];

    //获取对应的图片上的坐标
    auto i = static_cast<int>(u * size[0]);
    auto j = static_cast<int>(v * size[1]); 

    //如果超出范围，重复边缘颜色
    i = Clamp(i, 0, size[0] - 1);
    j = Clamp(j, 0, size[1] - 1);

    //获取spectrum表示（转换到[0,1]范围）
    auto r = static_cast<int>(data[3 * i + 3 * size[0] * j + 0]) / 255.f;
    auto g = static_cast<int>(data[3 * i + 3 * size[0] * j + 1]) / 255.f;
    auto b = static_cast<int>(data[3 * i + 3 * size[0] * j + 2]) / 255.f;

    return Spectrum(r, g, b);
}

Spectrum ImageTexture::evaluate(const Intersection &intersection) const{
    auto texCoord = mapping->map(intersection);
    return evaluate(texCoord, intersection.position);
}