#pragma once
#include"CoreLayer/Math/Math.h"

class Sampler{
public:
    Sampler() = default;
    Sampler(int x_samples, int y_samples)
        : xSamples(x_samples), ySamples(y_samples) {}

    virtual float sampler1D() = 0;      //��ȡһά��[0,1]�����
    virtual Vector2f sampler2D() = 0;   //��ȡ��ά��[0,1]�����

public:
    int xSamples, ySamples;
};