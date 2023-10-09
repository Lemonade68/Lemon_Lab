#include"IndependentSampler.h"

IndependentSampler::IndependentSampler(int x_samples, int y_samples) : Sampler(x_samples,y_samples){
    gen = std::mt19937(rd());       //����һ�����uint�����������
    dist = std::uniform_real_distribution<float>(.0f, 1.f);     //0.0��1.0�����������
}

float IndependentSampler::sampler1D(){
    return dist(gen);
}

Vector2f IndependentSampler::sampler2D(){
    return Vector2f(dist(gen), dist(gen));
}

