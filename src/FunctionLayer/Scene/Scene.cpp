#include"Scene.h"
#include"CoreLayer/Math/Function.h"

std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    bool hit = false, result = false;
    Intersection intersection;
    for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
        result = (*it)->rayIntersectShape(ray, intersection);       //�����������������
        if(result)
            hit = result;
    }
    if(!hit)
        return std::nullopt;

    return std::make_optional(intersection);
}

//������ֹ�segmentation fault
std::shared_ptr<Light> Scene::sampleLight(float sample, float &pdf) const{
    if(light_num == 0)                  //û�й�Դ��ֱ�ӷ���
        return nullptr;
    float p = 1.f / light_num;
    pdf = p;                            //���Ȳ�����Դ����pdfֱ�ӷ���p
    
    int i = (int)(sample / p);          //ת��Ϊ��������ΧӦ����[0, light_num]
    Clamp(i, 0, light_num - 1);         //i�ķ�Χ������[0,light_num - 1]��
    //���ص�i����Դ                     //������ĵط�������(i��int���ͣ�û����ʽ��ʼ������������ֵ)
    return light_list[i];
}