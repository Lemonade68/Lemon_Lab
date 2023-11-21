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
#include"ResourceLayer/Image.h"

#include"FunctionLayer/Shape/Triangle.h"
#include"FunctionLayer/Material/Conductor.h"
#include"FunctionLayer/Material/Dielectric.h"


#include<iostream>
#include<stdio.h>
#include<chrono>
#include<vector>
#include<memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//д��hdr��ʽ(��image�ж���)
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb/stb_image_write.h>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800
#define SPP 10
#define MAX_DEPTH 5

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void printProgress(float percentage) {
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
    auto camera = PinholeCamera(aspect_ratio, 45, Point3f(.0f, .5f, 6.f), Point3f(.0f,.5f,.0f));     // cornell boxʹ��

    auto sampler = std::make_shared<IndependentSampler>();

    // auto integrator = std::make_shared<NormalIntegrator>();
    // auto integrator = std::make_shared<DirectIntegrator_SampleBSDF>();  //��bsdf������ֱ�ӻ���������ǿ����һ����������
    // auto integrator = std::make_shared<DirectIntegrator_SampleLight>();  //�Թ�Դ������ֱ�ӻ�������Ŀǰ�����segmentation fault
    // auto integrator = std::make_shared<WhittedIntegrator>();
    auto integrator = std::make_shared<PathIntegrator>(max_depth);     //��������10

    //���ڱ���HDRͼƬ
    Image hdrImage(Vector2i(SCR_WIDTH, SCR_HEIGHT));

    //������������===================================================
    Scene scene;

    // cornell box���� ===========================
    //����
    Parallelogram obj1(Point3f(-2.5f, -1.f, -1.5f), Vector3f(.0f, .0f, 3.f), Vector3f(5.f, .0f, .0f));
    scene.addObject(std::make_shared<Parallelogram>(obj1));

    //ͷ��
    Parallelogram obj2(Point3f(-2.5f, 2.f, -1.5f), Vector3f(5.f, .0f, .0f), Vector3f(.0f, .0f, 3.f), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.8f))));
    scene.addObject(std::make_shared<Parallelogram>(obj2));

    //����
    Parallelogram obj3(Point3f(-2.5f, -1.f, -1.5f), Vector3f(.0f, 3.f, .0f), Vector3f(.0f, .0f, 3.f), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.7f,.3f,.3f))));
    scene.addObject(std::make_shared<Parallelogram>(obj3));

    //����
    Parallelogram obj4(Point3f(2.5f, -1.f, -1.5f), Vector3f(.0f, 3.f, .0f), Vector3f(.0f, .0f, 3.f), nullptr, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.3f,.7f,.3f))));
    scene.addObject(std::make_shared<Parallelogram>(obj4));

    //����
    Parallelogram obj5(Point3f(2.5f, -1.f, -1.5f), Vector3f(.0f, 3.f, .0f), Vector3f(-5.f, .0f, .0f));
    scene.addObject(std::make_shared<Parallelogram>(obj5));

    // Cube cube1(Point3f(-1.f), Point3f(1.f), std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f))), nullptr, Vector3f(-0.5, -0.2, -0.6), Vector3f(0.45, 0.8, 0.45), Vector3f(0, 1, 0), .5f);
    // // Cube cube1(Point3f(-1.f), Point3f(1.f), std::make_shared<Mirror_Material>(Spectrum(.6f)), nullptr, Vector3f(-0.5, -0.2, -0.6), Vector3f(0.45, 0.8, 0.45), Vector3f(0, 1, 0), .5f);
    // scene.addObject(std::make_shared<Cube>(cube1));

    // Cube cube2(Point3f(-1.f), Point3f(1.f), std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(.5f))), nullptr, Vector3f(0.6, -0.6, 0.3), Vector3f(0.3, 0.4, 0.3), Vector3f(0, 1, 0), -.5f);
    // scene.addObject(std::make_shared<Cube>(cube2));

    //ģ�͵���
    // std::string bunnyPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/bunny/bunny.obj");
    // std::string modelPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/utah_teapot/teapot.obj");        //һ��obj�ļ���������mesh��ģ��̫���ˣ�����ϵ��Ҫ.015�ſ���
    // std::string modelPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/viking_room/viking_room.obj");
    // std::string modelPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/dragon/dragon.obj");
    // std::string modelPath1("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/disney_bsdf/obj2.obj");
    // std::string modelPath2("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/disney_bsdf/obj3.obj");
    // std::string monkeyPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/monkey/suzanne.obj");
    // std::string dragonPath("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/Models/dragon/dragon.obj");

    // int nx, ny, nrChannels;
    // // ʹ�����·�������е�����
    // unsigned char *data = stbi_load("C:/Users/Lemonade/Desktop/Lemon_Lab/src/ResourceLayer/models/viking_room/viking_room.png", &nx, &ny, &nrChannels, 0);
    // if(!data)
    //     std::cerr << "Failed to load image!" << std::endl;

    int maxLeafSize = 1;
    // TriangleMesh model(modelPath, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(1.f, .98f, .94f))));
    // TriangleMesh model2(modelPath1, maxLeafSize, std::make_shared<Mirror_Material>(Spectrum(.5f)), nullptr, Vector3f(-.1f, -.3f, .0f), Vector3f(.4f));
    // TriangleMesh model1(modelPath2, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(1.f, .98f, .94f))), nullptr, Vector3f(-.1f, -.3f, .0f), Vector3f(.4f));
    // TriangleMesh model(modelPath, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(1.f, .98f, .94f))), nullptr, Vector3f(.2f, -1.f, .0f));
    // TriangleMesh model(modelPath, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(1.f, .98f, .94f))), nullptr, Vector3f(.0, -1.f, .0f), Vector3f(.015f));
    // TriangleMesh model(modelPath, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(1.f, .98f, .94f))), nullptr, Vector3f(-.1f, -.45f, .0f), Vector3f(2.f), Vector3f(.0f, 1.f, .0f), 2.f * PI / 3.f);
    // TriangleMesh model(modelPath, maxLeafSize, std::make_shared<Matte_Material>(std::make_shared<ImageTexture>(data, nx, ny)), nullptr, Vector3f(.0f, .0f, .0f), Vector3f(1.f), Vector3f(1.f, 1.f, .0f), -45.f);
    // TriangleMesh bunny(bunnyPath, maxLeafSize, std::make_shared<Conductor_Material>(.005f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(-.8f, -1.f, .0f));
    // TriangleMesh monkey(monkeyPath, maxLeafSize, std::make_shared<Conductor_Material>(.3f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(1.f, -.2f, .0f), Vector3f(.7f));
    // TriangleMesh dragon(dragonPath, maxLeafSize, std::make_shared<Conductor_Material>(.2f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(1.f, -.45f, .2f), Vector3f(2.f), Vector3f(.0f, 1.f, .0f), -2.f * PI / 3.f);

    // scene.addObject(std::make_shared<TriangleMesh>(bunny));
    // scene.addObject(std::make_shared<TriangleMesh>(monkey));
    // scene.addObject(std::make_shared<TriangleMesh>(dragon));
    // scene.addObject(std::make_shared<TriangleMesh>(model2));
    
    // Sphere sphere1(Point3f(.0f), .35f, std::make_shared<Conductor_Material>(.005f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(-1.8f, .8f, .0f));
    // scene.addObject(std::make_shared<Sphere>(sphere1));

    // Sphere sphere2(Point3f(.0f), .35f, std::make_shared<Conductor_Material>(.01f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(-.9f, .8f, .0f));
    // scene.addObject(std::make_shared<Sphere>(sphere2));

    // Sphere sphere3(Point3f(.0f), .35f, std::make_shared<Conductor_Material>(.1f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(.0f, .8f, .0f));
    // scene.addObject(std::make_shared<Sphere>(sphere3));

    // Sphere sphere4(Point3f(.0f), .35f, std::make_shared<Conductor_Material>(.3f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(.9f, .8f, .0f));
    // scene.addObject(std::make_shared<Sphere>(sphere4));

    // Sphere sphere5(Point3f(.0f), .35f, std::make_shared<Conductor_Material>(.65f, Vector3f(0.15494, 0.11648, 0.13809), Vector3f(4.81810, 3.11562, 2.14240)), nullptr, Vector3f(1.8f, .8f, .0f));
    // scene.addObject(std::make_shared<Sphere>(sphere5));

    Sphere sphere6(Point3f(.0f), .35f, std::make_shared<Dielectric_Material>(.05f, Vector3f(.5f)),nullptr, Vector3f(-1.8f, -.2f, .0f));
    scene.addObject(std::make_shared<Sphere>(sphere6));

    Sphere sphere7(Point3f(.0f), .35f, std::make_shared<Dielectric_Material>(.2f, Vector3f(.5f)), nullptr, Vector3f(-.9f, -.2f, .0f));
    scene.addObject(std::make_shared<Sphere>(sphere7));

    Sphere sphere8(Point3f(.0f), .35f, std::make_shared<Dielectric_Material>(.4f, Vector3f(.5f)), nullptr, Vector3f(.0f, -.2f, .0f));
    scene.addObject(std::make_shared<Sphere>(sphere8));

    Sphere sphere9(Point3f(.0f), .35f, std::make_shared<Dielectric_Material>(.55f, Vector3f(.5f)), nullptr, Vector3f(.9f, -.2f, .0f));
    scene.addObject(std::make_shared<Sphere>(sphere9));

    Sphere sphere10(Point3f(.0f), .35f, std::make_shared<Dielectric_Material>(.75f, Vector3f(.5f)), nullptr, Vector3f(1.8f, -.2f, .0f));
    scene.addObject(std::make_shared<Sphere>(sphere10));

    Parallelogram light1(Point3f(-0.6, 1.98, -0.6), Vector3f(1.2, 0, 0), Vector3f(0, 0, 1.2));
    scene.addLight(std::make_shared<AreaLight>(Spectrum(3.f), std::make_shared<Parallelogram>(light1)));

    // Parallelogram light2(Point3f(-0.6, -0.98, -0.6), Vector3f(1.2, 0, 0), Vector3f(0, 0, 1.2));
    // scene.addLight(std::make_shared<AreaLight>(Spectrum(3.f), std::make_shared<Parallelogram>(light2)));

    //==========================================

    //�������ٽṹ
    scene.buildSceneAcceleration();
    // scene.debugPrintAccleration();

    //�Ƿ���debug
    // bool debug = true;
    bool debug = false;

    //�Ƿ����ΪhdrͼƬ
    bool isHdr = false;
    // bool isHdr = true;

    if(!debug){
        auto start = std::chrono::system_clock::now();

        if(!isHdr)
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

                //hdrͼƬ
                if(isHdr)
                    hdrImage.setValue(Vector2i(x, y), s2v(li));
                else{
                    //ע���������ض���Χ[0,1]�ڣ�hdrͼƬ��˵��
                    int ir = static_cast<int>(Clamp(li.x(), .0f, 1.f) * 255.99f);
                    int ig = static_cast<int>(Clamp(li.y(), .0f, 1.f) * 255.99f);
                    int ib = static_cast<int>(Clamp(li.z(), .0f, 1.f) * 255.99f);
                    std::cout << ir << ' ' << ig << ' ' << ib << '\n';
                }

                //��ʱ����ʹ�ã���ΪĿǰ��ֱ�������ppm�ļ����
                // int finished_num = x + y * image_width;
                // if(finished_num % 5 == 0)
                //     printProgress((float)finished_num / (image_height * image_width));
            }
            std::cerr << "\r"<< image_height - y - 1 << " lines remains ";      //�����ӿո��ֹs���ӡ
            fflush(stdout);
        }

        //���ΪHDRͼƬʱʹ��
        if(isHdr)
            hdrImage.saveHDR("testingHdr.hdr");

        std::cerr << "\nDone!" << std::endl;

        // printProgress(1.0f);    //��ֹ��󵽲���100%
        auto end = std::chrono::system_clock::now();
        std::cerr << "Rendering costs " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / 1000.f << "s\n";
    }

}
    