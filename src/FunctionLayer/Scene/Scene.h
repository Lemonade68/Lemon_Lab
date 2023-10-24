#pragma once

#include"CoreLayer/Math/Math.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Shape/Shape.h"
#include"FunctionLayer/Shape/Intersection.h"
#include"FunctionLayer/Light/AreaLight.h"

#include"FunctionLayer/Acceleration/Acceleration.h"      //��ʱĬ��ʹ��BVH����

#include<optional>
#include<vector>
#include<memory>


//��ʱδ���Ǽ��ٽṹ(���ѿ���)
class Scene {
public:
    Scene() { 
        light_num = 0;
        acceleration = nullptr;
    }

    Scene(std::vector<std::shared_ptr<Shape>> _shape_list, std::vector<std::shared_ptr<Light>> _light_list)
        : shape_list(_shape_list), light_list(_light_list), light_num(_light_list.size()) {}

    //�������
    void addObject(const std::shared_ptr<Shape> &ptr){
        shape_list.push_back(ptr);
    }

    //��ӹ�Դ�����Դ�Ļ�����shape����Է������Ҳ��ӵ�shape_list�У�
    void addLight(const std::shared_ptr<Light> &light){
        if(light->type ==LightType::EnvironmentLight)       //������Դ���������ģ�
            return;
        //���Դ��Ҳ��ӵ������б��У���Ϊ�������Ҳ�����ڻ�������ʾ��
        if(light->type == LightType::AreaLight){
            //ѧϰ������д��������û�е���Ҫ��������Ա��
            auto shape = std::static_pointer_cast<AreaLight>(light)->shape;
            shape->light = light;
            shape_list.push_back(shape);
        }
        
        light_list.push_back(light);
        ++light_num;
    }

    //���������ļ��ٽṹ
    void buildSceneAcceleration();

    //�����볡���󽻣���Ӽ��ٽṹ�����仯��
    std::optional<Intersection> rayIntersect(Ray &ray) const;

    //TODO����ӻ�����
    // std::vector<std::shared_ptr<InfiniteLight>> InfiniteLights;

    //�ӳ����в���һ����Դ�����ظù�Դ��ָ�룬�����ز������ù�Դ�ĸ��ʣ�Ĭ��Ϊ���Ȳ�����
    std::shared_ptr<Light> sampleLight(float sample, float &pdf) const;


    void debugPrintLight(){
        std::cout << "Light Information:==============================\n";
        std::cout << "light number: " << light_num << std::endl;
        for (auto it = light_list.begin(); it != light_list.end(); ++it){
            (*it)->debugPrint();
        }
        std::cout << "================================================\n";
    }

    //��ӡ������Ϣ�����������Ҳ��ӡ������Ϣ
    void debugPrintObject(){
        std::cout << "Object Information:==============================\n";
        for (auto it = shape_list.begin(); it != shape_list.end(); ++it){
            (*it)->debugPrint();
            if((*it)->light){
                printf("- Has light propertity, parameter:\n");
                (*it)->light->debugPrint();
            }
        }
        std::cout << "=================================================\n";
    }

    void debugPrintAccleration(){
        acceleration->debugPrint();
    }

private:
    std::vector<std::shared_ptr<Shape>> shape_list;
    std::vector<std::shared_ptr<Light>> light_list;     //��Դ�б�

    std::shared_ptr<Acceleration> acceleration;         //���ٽṹ

    int light_num;              //��������Ҫ�ǲ���ʽ��ʼ���Ļ���������ֵ

    //TODO������acceleration
};