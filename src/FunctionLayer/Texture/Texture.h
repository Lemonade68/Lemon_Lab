#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Shape/Intersection.h"


struct TextureCoord{
    Vector2f coord;
    //TODO��΢�ֹ��ߵ��������
};


//��ȡ���������Ӧ����ɫֵ������һ���࣬�պ󷽱�ʵ�ַǶ�ά������
class TextureMapping{
public:
    virtual TextureCoord map(const Intersection &intersection) const = 0;
};


//���ݶ�ά������������ȡ��ɫֵ
class UVMapping : public TextureMapping {
public:
    virtual TextureCoord map(const Intersection &intersection) const override;
};


//Texture�࣬ʹ����ģ����Է���floatֵ��Ҳ���Է���spectrumֵ
template <typename TReturn> class Texture{
public:
    Texture() { mapping = std::make_shared<UVMapping>(); }  //Ĭ��ʹ�ö�ά����ӳ��

    //ʹ����������ӳ��
    //...

    //���ݶ�ά������������ȡ��ɫֵ
    virtual TReturn evaluate(const TextureCoord &texCoord) const = 0;

    //���ݽ�����Ϣ��ȡ��ɫֵ����ʵ����ͨ��mapping�������texCoord���ٴ������棩
    virtual TReturn evaluate(const Intersection &intersection) const = 0;

public:
    Vector2i size;      //�����С

protected:
    std::shared_ptr<TextureMapping> mapping;    //����ӳ���Ա����intersection����Ϣת��Ϊ�������ʹ�õ�TexCoord

};
