#pragma once
#include"Sampler.h"
#include<random>

class IndependentSampler : public Sampler{
public:
    IndependentSampler(int x_samples = 8, int y_samples = 8);

    virtual float sampler1D() override;     //返回0-1的一维随机浮点数

    virtual Vector2f sampler2D() override;  //返回0-1的二维随机浮点数向量

private:
    //C11中产生随机数的对象，随机生成0-2^32中的数，作为gen的种子
    std::random_device rd;

    //伪随机数产生器，返回uint
    std::mt19937 gen;  

    //指定范围的随机实数（均匀分布）
    std::uniform_real_distribution<float> dist;
    //std::uniform_int_distribution：指定范围的随机非负数
};