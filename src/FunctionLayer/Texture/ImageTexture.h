#pragma once

#include"Texture.h"
#include"CoreLayer/ColorSpace/Spectrum.h"

//��ʱֱ��ʹ�ü򵥵�unsigned char���洢��Ϣ�������image�࣬�������ع�
//ͼƬ����Ļ��϶��õ���spectrum���õ���Ϣ��ֱ��ָ��TReturn��ֵ
class ImageTexture : public Texture<Spectrum>{
public:
    ImageTexture() = default;
    ImageTexture(unsigned char *_data, int nx, int ny){
        data = _data;
        size = Vector2i(nx, ny);
    }

    ~ImageTexture() { delete data; }        //����ʱ�ǵ�ɾ��ָ���Ӧ�ڴ�

    virtual Spectrum evaluate(const TextureCoord &texCoord) const override;

    virtual Spectrum evaluate(const Intersection &intersection) const override;

private:
    //��С�Ѿ���Texture���ж���
    unsigned char *data;        //unsigned char�洢��ɫ��8bit��Ӧ256λ��RGb
};