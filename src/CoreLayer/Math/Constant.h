#pragma once

#include<cmath>
#include<limits>

#ifdef FLT_MAX
#undef FLT_MAX
#endif      //防止下面的FLT_MAX无法被声明（系统自带FLT_MAX）

#define PI 3.14159265358979323846
#define EPSILON 1e-4

constexpr float INV_PI = 1.f / PI;
constexpr float FLT_MAX = std::numeric_limits<float>::max();