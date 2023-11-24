#include"ImageTexture.h"

//ֱ�Ӵ����������ȡͼƬ�������Ϣ
Spectrum ImageTexture::evaluate(const TextureCoord &texCoord, const Point3f &position) const{
    if(data == nullptr)
        return Spectrum(1.f, .0f, .0f);     //���ش���ɫ����ʾͼƬ���ش���

    auto u = texCoord.coord[0];        //float����
    auto v = texCoord.coord[1];

    //��ȡ��Ӧ��ͼƬ�ϵ�����
    auto i = static_cast<int>(u * size[0]);
    auto j = static_cast<int>(v * size[1]); 

    //���������Χ���ظ���Ե��ɫ
    i = Clamp(i, 0, size[0] - 1);
    j = Clamp(j, 0, size[1] - 1);

    //��ȡspectrum��ʾ��ת����[0,1]��Χ��
    auto r = static_cast<int>(data[3 * i + 3 * size[0] * j + 0]) / 255.f;
    auto g = static_cast<int>(data[3 * i + 3 * size[0] * j + 1]) / 255.f;
    auto b = static_cast<int>(data[3 * i + 3 * size[0] * j + 2]) / 255.f;

    return Spectrum(r, g, b);
}

Spectrum ImageTexture::evaluate(const Intersection &intersection) const{
    auto texCoord = mapping->map(intersection);
    return evaluate(texCoord, intersection.position);
}