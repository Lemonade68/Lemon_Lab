#pragma once

#include<cmath>
#include<limits>

#ifdef FLT_MAX
#undef FLT_MAX
#endif      //��ֹ�����FLT_MAX�޷���������ϵͳ�Դ�FLT_MAX��

#define PI 3.14159265358979323846
#define EPSILON 1e-4

constexpr float INV_PI = 1.f / PI;
constexpr float FLT_MAX = std::numeric_limits<float>::max();