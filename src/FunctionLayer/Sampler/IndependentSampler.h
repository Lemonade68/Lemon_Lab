#pragma once
#include"Sampler.h"
#include<random>

class IndependentSampler : public Sampler{
public:
    IndependentSampler(int x_samples = 8, int y_samples = 8);

    virtual float sampler1D() override;     //����0-1��һά���������

    virtual Vector2f sampler2D() override;  //����0-1�Ķ�ά�������������

private:
    //C11�в���������Ķ����������0-2^32�е�������Ϊgen������
    std::random_device rd;

    //α�����������������uint
    std::mt19937 gen;  

    //ָ����Χ�����ʵ�������ȷֲ���
    std::uniform_real_distribution<float> dist;
    //std::uniform_int_distribution��ָ����Χ������Ǹ���
};