#pragma once
#include<cmath>      //constant中包含(?)
#include<algorithm>  //geometry中包含

//限制范围在[lower到upper]内
template<typename T> T Clamp (T t, T lower, T upper){
    t = (t < lower) ? lower : (t > upper ? upper : t);
    return t;
}

//求解二元一次方程组，返回值永远是  x1 < x2
inline bool Quadratic(float A, float B, float C, float *x1, float *x2){
    float discrim = B * B - 4 * A * C;
    if(discrim < 0)
        return false;

    float rootDiscrim = std::sqrt(discrim);
    float q;
    if(B < 0)
        q = -.5f * (B - rootDiscrim);
    else
        q = -.5f * (B + rootDiscrim);
    *x1 = q / A;
    *x2 = C / q;        //平方差公式推导一下，也对

    if(*x1 > *x2)
        std::swap(*x1, *x2);
    
    return true;
}