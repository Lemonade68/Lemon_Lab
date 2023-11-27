#include"Scene.h"
#include"CoreLayer/Math/Function.h"
#include"FunctionLayer/Acceleration/BVH.h"
#include"FunctionLayer/Acceleration/EmbreeBVH.h"

//���˰�Χ�к�������
void Scene::buildSceneAcceleration(){
    // acceleration = std::make_shared<BVH>(shape_list, 1);   //��ȡ�����б�
    acceleration = std::make_shared<EmbreeBVH>(shape_list);   //��ȡ�����б�
    acceleration->build();      //����BVH���ٽṹ
}

//���˼��ٽṹ���д��
std::optional<Intersection> Scene::rayIntersect(Ray &ray) const{
    return acceleration->rayIntersect(ray);
}

//������ֹ�segmentation fault
std::shared_ptr<Light> Scene::sampleLight(float sample, float &pdf) const{
    if(light_num == 0)                  //û�й�Դ��ֱ�ӷ���
        return nullptr;

    float p = 1.f / light_num;
    pdf = p;                            //���Ȳ�����Դ����pdfֱ�ӷ���p

    int i = static_cast<int>(sample / p);   // ת��Ϊ��������ΧӦ����[0, light_num]
    
    // �ú�����Ҫʹ�÷���ֵ��������ֵ������Ĳ������ã�
    i = Clamp(i, 0, light_num - 1);             //i�ķ�Χ������[0,light_num - 1]��
    
    //���ص�i����Դ                       //������ĵط�������(i��int���ͣ�û����ʽ��ʼ������������ֵ)
    return light_list[i];
}