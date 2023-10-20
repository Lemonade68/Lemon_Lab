#include"CoreLayer/Math/Math.h"
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Camera/Pinhole.h"
#include"FunctionLayer/Shape/Sphere.h"
#include"FunctionLayer/Sampler/IndependentSampler.h"
#include"FunctionLayer/Integrator/NormalIntegrator.h"
#include"FunctionLayer/Shape/Parallelogram.h"
#include"FunctionLayer/Scene/Scene.h"

#include"FunctionLayer/Material/Matte.h"
#include"FunctionLayer/Light/AreaLight.h"
#include"FunctionLayer/Integrator/DirectIntegrator.h"

#include"FunctionLayer/Film/Film.h"
#include"FunctionLayer/Material/Mirror.h"
// #include"FunctionLayer/Texture/ConstantTexture.h"        //Matte���Ѿ�����
#include"FunctionLayer/Texture/ImageTexture.h"

#include"FunctionLayer/Integrator/WhittedIntegrator.h"
#include"FunctionLayer/Shape/Cube.h"

#include"FunctionLayer/Integrator/PathIntegrator.h"

#include<iostream>
#include<stdio.h>
#include<chrono>
#include<vector>
#include<memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#define SCR_WIDTH 1200
#define SCR_HEIGHT 800
#define SPP 5
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

    // auto camera = PinholeCamera(aspect_ratio, fov);                     //��������ʹ��Ĭ�ϲ���
    auto camera = PinholeCamera(aspect_ratio, 45, Point3f(.0f, .5f, 6.f),Point3f(.0f,.5f,.0f));     // cornell boxʹ��

    auto sampler = std::make_shared<IndependentSampler>();

    // auto integrator = std::make_shared<NormalIntegrator>();
    // auto integrator = std::make_shared<DirectIntegrator_SampleBSDF>();  //��bsdf������ֱ�ӻ���������ǿ����һ����������
    // auto integrator = std::make_shared<DirectIntegrator_SampleLight>();  //�Թ�Դ������ֱ�ӻ�������Ŀǰ�����segmentation fault
    // auto integrator = std::make_shared<WhittedIntegrator>();
    auto integrator = std::make_shared<PathIntegrator>(10);

    //������������===================================================
    Scene scene;

    // cornell box���� ===========================
    // ����
    Parallelogram obj1(Point3f(-1.5, -1, -1.5), Vector3f(0, 0, 3), Vector3f(3, 0, 0));  
    scene.addObject(std::make_shared<Parallelogram>(obj1));

    //ͷ��
    Parallelogram obj2(Point3f(-1.5, 2, -1.5), Vector3f(3, 0, 0), Vector3f(0, 0, 3), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.9f))));
    scene.addObject(std::make_shared<Parallelogram>(obj2));

    //����
    Parallelogram obj3(Point3f(-1.5, -1, -1.5), Vector3f(0, 3, 0), Vector3f(0, 0, 3), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.7f,.3f,.3f))));
    scene.addObject(std::make_shared<Parallelogram>(obj3));

    //����
    Parallelogram obj4(Point3f(1.5, -1, -1.5), Vector3f(0, 3, 0), Vector3f(0, 0, 3), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.3f,.7f,.3f))));
    scene.addObject(std::make_shared<Parallelogram>(obj4));

    //����
    Parallelogram obj5(Point3f(1.5, -1, -1.5), Vector3f(0, 3, 0), Vector3f(-3, 0, 0));
    scene.addObject(std::make_shared<Parallelogram>(obj5));

    Cube cube1(Point3f(-1.f), Point3f(1.f), std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f))), nullptr, Vector3f(-0.5, -0.2, -0.6), Vector3f(0.45, 0.8, 0.45), Vector3f(0, 1, 0), .5f);
    scene.addObject(std::make_shared<Cube>(cube1));

    Cube cube2(Point3f(-1.f), Point3f(1.f), std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f))), nullptr, Vector3f(0.6, -0.6, 0.3), Vector3f(0.3, 0.4, 0.3), Vector3f(0, 1, 0), -.5f);
    scene.addObject(std::make_shared<Cube>(cube2));

    Parallelogram light1(Point3f(-0.3, 1.99, -0.3), Vector3f(0.6, 0, 0), Vector3f(0, 0, 0.6));
    scene.addLight(std::make_shared<AreaLight>(Spectrum(45.f), std::make_shared<Parallelogram>(light1)));

    //==========================================

    // Sphere sphere();     //����д�޷������������ĺ����������Ƕ������

    // Sphere sphere;
    // scene.addObject(std::make_shared<Sphere>(sphere));

    // Sphere sphere(Point3f(1.f, .0f, .0f), 1.f, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.1f, .5f, .6f))));
    // scene.addObject(std::make_shared<Sphere>(sphere));

    // Sphere sphere1(Point3f(-1.f, 0.f, .0f), 1.f, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f, .1f, .1f))));       //��ɫС��
    // scene.addObject(std::make_shared<Sphere>(sphere1));

    // Sphere sphere2(Point3f(1.f, 0.f, .0f), 1.f, std::make_shared<Mirror_Material>(Spectrum(.6f)));
    // scene.addObject(std::make_shared<Sphere>(sphere2));

    // Sphere sphere3(Point3f(1.f, 0.f, .0f), 1.f);
    // scene.addObject(std::make_shared<Sphere>(sphere3));

    // Parallelogram obj1(Point3f(-1.f, 0.f, 1.f), Vector3f(2.f, -1.f, .0f), Vector3f(.0f, .0f, -2.f));
    // scene.addObject(std::make_shared<Parallelogram>(obj1));

    //������
    // Cube cube1(Point3f(-1.f), Point3f(1.f), std::make_shared<Matte_Material>(), nullptr, Vector3f(-.5f,.0f,.0f), Vector3f(.5f), Vector3f(1.f, 1.f, .0f), 45.f);
    // scene.addObject(std::make_shared<Cube>(cube1));

    // Cube cube2(Point3f(-1.f), Point3f(1.f), std::make_shared<Mirror_Material>(Spectrum(.6f)), nullptr, Vector3f(1.f, .0f, .0f), Vector3f(.5f), Vector3f(1.f, 1.f, .0f), 70.f);
    // scene.addObject(std::make_shared<Cube>(cube2));

    //ͼƬ������/ƽ��
    // int nx, ny, nrChannels;
    // //ʹ�����·�������е�����
    // unsigned char *data = stbi_load("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Textures/earth.jpg", &nx, &ny, &nrChannels, 0);
    // if(!data)
    //     std::cerr << "Failed to load image!" << std::endl;
    // Sphere sphere_image(Point3f(-1.f, .0f, .0f), 1.f, std::make_shared<Matte_Material>(std::make_shared<ImageTexture>(data, nx, ny)));
    // scene.addObject(std::make_shared<Sphere>(sphere_image));
    // Parallelogram para_image(Point3f(-1.f, .5f, 1.f), Vector3f(2.f, .0f, .0f), Vector3f(.0f, -1.f, .0f), nullptr, std::make_shared<Matte_Material>(std::make_shared<ImageTexture>(data, nx, ny)));
    // scene.addObject(std::make_shared<Parallelogram>(para_image));

    //ƽ��  
    // Parallelogram paral1(Point3f(-80.f, -1.f, 80.f), Vector3f(160.f, .0f, .0f), Vector3f(.0f, 0.f, -160.f));
    // scene.addObject(std::make_shared<Parallelogram>(paral1));

    //ͷ��
    // Parallelogram light1(Point3f(-2.f, 2.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(4.f, .0f, .0f));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(5.f), std::make_shared<Parallelogram>(light1)));

    //�ҷ�
    // Parallelogram light2(Point3f(2.f, 1.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(.0f, -2.f, .0f));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(5.f), std::make_shared<Parallelogram>(light2)));

    //��
    // Parallelogram light3(Point3f(2.f, 1.f, -2.f), Vector3f(-4.f, .0f, .0f), Vector3f(.0f, -2.f, .0f));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(5.f), std::make_shared<Parallelogram>(light3)));

    //��
    // Parallelogram light4(Point3f(-2.f, 1.f, 1.f), Vector3f(.0f, .0f, -2.f), Vector3f(.0f, -2.f, .0f));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(5.f), std::make_shared<Parallelogram>(light4)));

    //ǰ��(����ʹ��)
    // Parallelogram light5(Point3f(-1.f, 0.f, 6.f), Vector3f(2.f, .0f, .0f), Vector3f(.0f, -1.f, .0f));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(5.f), std::make_shared<Parallelogram>(light5)));

    //����debug��ӡ��Ϣ
    // scene.debugPrintLight();
    // scene.debugPrintObject();

    // Sphere light3(Point3f(.0f, 0.0f, 0.0f), 3.0f, std::make_shared<Matte_Material>(), std::make_shared<AreaLight>());
    // scene.addLight(std::make_shared<Sphere>(light3));

    //================================================================

    //�Ƿ���debug
    // bool debug = true;
    bool debug = false;

    if(!debug){
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

                //ע���������ض���Χ[0,1]�ڣ�hdrͼƬ��˵��
                int ir = static_cast<int>(Clamp(li.x(), .0f, 1.f) * 255.99f);
                int ig = static_cast<int>(Clamp(li.y(), .0f, 1.f) * 255.99f);
                int ib = static_cast<int>(Clamp(li.z(), .0f, 1.f) * 255.99f);
                std::cout << ir << ' ' << ig << ' ' << ib << '\n';

                //��ʱ����ʹ�ã���ΪĿǰ��ֱ�������ppm�ļ����
                // int finished_num = x + y * image_width;
                // if(finished_num % 5 == 0)
                //     printProgress((float)finished_num / (image_height * image_width));
            }
            std::cerr << "\r"<< image_height - y - 1 << " lines remains ";      //�����ӿո��ֹs���ӡ
            fflush(stdout);
        }

        std::cerr << "\nDone!" << std::endl;

        // printProgress(1.0f);    //��ֹ��󵽲���100%
        auto end = std::chrono::system_clock::now();
        std::cerr << "Rendering costs " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / 1000.f << "s\n";
    }

}
    