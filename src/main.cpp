#include"CoreLayer/Math/Math.h"
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Camera/Pinhole.h"
#include"FunctionLayer/Shape/Sphere.h"
#include"FunctionLayer/Sampler/IndependentSampler.h"
#include"FunctionLayer/Integrator/NormalIntegrator.h"
#include"FunctionLayer/Shape/Parallelogram.h"
#include"FunctionLayer/Scene/Scene.h"

#include"FunctionLayer/Material/BxDF/Lambert.h"

#include<iostream>
#include<stdio.h>
#include<chrono>
#include<vector>
#include<memory>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800
#define SPP 10
#define MAX_DEPTH 10

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void printProgress(float percentage){
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    
    // https://blog.csdn.net/qq_18841761/article/details/107571092
    // \r��ÿ�δӵ�ǰ�е�ͷ��ʼ���������%3d��valָ����%%�������%
    // %.*s���洫���������������Ⱥ��ַ�������ʾ�����ô�೤�ȵ��ַ�������
    // %*s��ʾ����ַ���s��������ռ���������λ�ã��������s��߲��ո���������൱���ǲ����˶�Ӧ�����Ŀո�
    // .ǰ���*��ʾ����ܿ�ȣ�.�����*��ʾ����ַ�����
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);   //��ջ���������ֱ���������ֹ�������ʱ��
}

int main(){
    const int image_width = SCR_WIDTH;
	const int image_height = SCR_HEIGHT;
	const auto aspect_ratio = double(image_width) / image_height;
	const int samples_per_pixel = SPP;		//�����������ڿ����
	const int max_depth = MAX_DEPTH;
	// const double fuzz = 0.0;
	float fov = 80.f;

    auto camera = PinholeCamera(aspect_ratio, fov);     //��������ʹ��Ĭ�ϲ���

    auto sampler = std::make_shared<IndependentSampler>();
    auto integrator = std::make_shared<NormalIntegrator>();

    //������������===================================================
    Scene scene;
    Sphere sphere1;
    Parallelogram paral1(Point3f(-10.f, -1.f, 10.f), Vector3f(20.f, .0f, .0f), Vector3f(.0f, .0f, -20.f));
    
    scene.add(std::make_shared<Sphere>(sphere1));
    scene.add(std::make_shared<Parallelogram>(paral1));
    //================================================================

    auto start = std::chrono::system_clock::now();

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y){
        for (int x = 0; x < image_width; ++x){
            Spectrum li(.0f);
            Vector2f NDC((float)x / image_width, (float)y / image_height);

            //ʹ��spp
            for (int i = 0; i < samples_per_pixel; ++i){
                Ray ray = camera.sampleRay(CameraSample{sampler->sampler2D()}, NDC);     //struct�������ʹ��{}�ķ�ʽ��ʼ����δָ��������Ĭ�ϳ�ʼ��
                li += integrator->li(ray, scene, sampler);
            }
            li = li / samples_per_pixel;        //���ֵ

            int ir = static_cast<int>(li.x() * 255.99f);
            int ig = static_cast<int>(li.y() * 255.99f);
            int ib = static_cast<int>(li.z() * 255.99f);
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';

            //��ʱ����ʹ�ã���ΪĿǰ��ֱ�������ppm�ļ����
            // int finished_num = x + y * image_width;
            // if(finished_num % 5 == 0)
            //     printProgress((float)finished_num / (image_height * image_width));
        }
        std::cerr << "\r"<< image_height - y - 1 << " lines remains";
        fflush(stdout);
    }

    std::cerr << "\nDone!" << std::endl;

    // printProgress(1.0f);    //��ֹ��󵽲���100%
    auto end = std::chrono::system_clock::now();
    std::cerr << "Rendering costs " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / 1000.f << "s\n";
}