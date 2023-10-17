#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Shape/Intersection.h"


struct TextureCoord{
    Vector2f coord;
    //TODO：微分光线的纹理添加
};


//获取纹理坐标对应的颜色值，定义一个类，日后方便实现非二维的纹理
class TextureMapping{
public:
    virtual TextureCoord map(const Intersection &intersection) const = 0;
};


//根据二维纹理坐标来获取颜色值
class UVMapping : public TextureMapping {
public:
    virtual TextureCoord map(const Intersection &intersection) const override;
};


//Texture类，使用类模板可以返回float值，也可以返回spectrum值
template <typename TReturn> class Texture{
public:
    Texture() { mapping = std::make_shared<UVMapping>(); }  //默认使用二维纹理映射

    //使用其他纹理映射
    //...

    //根据二维纹理坐标来获取颜色值
    virtual TReturn evaluate(const TextureCoord &texCoord) const = 0;

    //根据交点信息获取颜色值（其实就是通过mapping方法求出texCoord，再带到上面）
    virtual TReturn evaluate(const Intersection &intersection) const = 0;

public:
    Vector2i size;      //纹理大小

protected:
    std::shared_ptr<TextureMapping> mapping;    //纹理映射成员，将intersection处信息转换为纹理计算使用的TexCoord

};
