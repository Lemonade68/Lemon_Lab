#include"FunctionLayer/Ray/Ray.h"
#include"CoreLayer/Math/Math.h"

#include<iostream>
#include<stdio.h>
#include<chrono>

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
	const double fuzz = 0.0;
	const double fov = 45;

    // auto start = std::chrono::system_clock::now();

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y){
        for (int x = 0; x < image_width; ++x){
            Vector3f li(.0f);
            //颜色映射到0-1，最后输出时再返回
            li.x() = float(x) / image_width;
            li.y() = float(y) / image_height;
            li.z() = .2f;

            int ir = static_cast<int>(li.x() * 255.999);
            int ig = static_cast<int>(li.y() * 255.999);
            int ib = static_cast<int>(li.z() * 255.999);
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
            
            //暂时不能使用：因为目前是直接输出到ppm文件里的
            // int finished_num = x + y * image_width;
            // if(finished_num % 5 == 0)
            //     printProgress((float)finished_num / (image_height * image_width));
        }
        std::cerr << "\r"<< image_height - y << " lines remains";
        fflush(stdout);
    }

    // printProgress(1.0f);    //防止最后到不了100%
    // auto end = std::chrono::system_clock::now();
    // printf("\nRendering costs %.2fs\n", (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
    //                                         .count() / 1000.f);

    std::cerr << "\nDone!" << std::endl;
}