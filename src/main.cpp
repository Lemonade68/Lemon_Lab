#include"CoreLayer/Math/Math.h"
#include"CoreLayer/ColorSpace/Spectrum.h"
#include"FunctionLayer/Ray/Ray.h"
#include"FunctionLayer/Camera/Pinhole.h"
#include"FunctionLayer/Shape/Sphere.h"
#include"FunctionLayer/Sampler/IndependentSampler.h"

#include<iostream>
#include<stdio.h>
#include<chrono>
#include<vector>
#include<memory>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800
#define SPP 100
#define MAX_DEPTH 10

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void printProgress(float percentage){
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    
    // https://blog.csdn.net/qq_18841761/article/details/107571092
    // \r：每次从当前行的头开始重新输出，%3d由val指定，%%输出的是%
    // %.*s后面传入两个参数：长度和字符串，表示输出这么多长度的字符串部分
    // %*s表示输出字符串s，且至少占后面参数个位置，不足的在s左边补空格（所以这边相当于是补上了对应个数的空格）
    // .前面的*表示输出总宽度，.后面的*表示输出字符个数
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);   //清空缓存区，并直接输出（防止输出不及时）
}

int main(){
    const int image_width = SCR_WIDTH;
	const int image_height = SCR_HEIGHT;
	const auto aspect_ratio = double(image_width) / image_height;
	const int samples_per_pixel = SPP;		//采样数，用于抗锯齿
	const int max_depth = MAX_DEPTH;
	// const double fuzz = 0.0;
	float fov = 80.f;

    auto camera = PinholeCamera(aspect_ratio, fov);     //其他参数使用默认参数

    IndependentSampler sampler;
    Sphere sphere1;
    std::vector<std::shared_ptr<Shape>> scene;
    scene.push_back(std::make_shared<Sphere>(sphere1));        //添加一个默认球体

    // // auto start = std::chrono::system_clock::now();

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y){
        for (int x = 0; x < image_width; ++x){
            // Spectrum li(.0f);
            Vector2f NDC((float)x / image_width, (float)y / image_height);

            Ray ray = camera.sampleRay(CameraSample{sampler.sampler2D()}, NDC);     //struct对象可以使用{}的方式初始化，未指定的属性默认初始化

            bool hit = false;
            Intersection intersection;

            //光线与场景中所有物体求交
            // for (auto it = scene.begin(); it != scene.end(); ++it){
            //     result = (*it)->rayIntersectShape(ray, intersection);
            //     if(result)
            //         hit = result;
            // }

            hit = sphere1.rayIntersectShape(ray, intersection);

            //先输出法线进行测试(记得对法线进行处理)
            if(hit){
                Vector3f normal = (intersection.normal + Vector3f(1.f)) * .5f;  //处理，防止负的法线数值
                int ir = static_cast<int>(normal.x() * 255.99f);
                int ig = static_cast<int>(normal.y() * 255.99f);
                int ib = static_cast<int>(normal.z() * 255.99f);
                std::cout << ir << ' ' << ig << ' ' << ib << '\n';
            }
            else
                std::cout << 0 << ' ' << 0 << ' ' << 0 << '\n';

            //暂时不能使用：因为目前是直接输出到ppm文件里的
            // int finished_num = x + y * image_width;
            // if(finished_num % 5 == 0)
            //     printProgress((float)finished_num / (image_height * image_width));
        }
        std::cerr << "\r"<< image_height - y - 1 << " lines remains";
        fflush(stdout);
    }

    // printProgress(1.0f);    //防止最后到不了100%
    // auto end = std::chrono::system_clock::now();
    // printf("\nRendering costs %.2fs\n", (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
    //                                         .count() / 1000.f);

    std::cerr << "\nDone!" << std::endl;
}