#pragma once
#include<cmath>      //constant�а���(?)
#include<algorithm>  //geometry�а���

//���Ʒ�Χ��[lower��upper]��
template<typename T> T Clamp (T t, T lower, T upper){
    t = (t < lower) ? lower : (t > upper ? upper : t);
    return t;
}

//����Ԫһ�η����飬����ֵ��Զ��  x1 < x2
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
    *x2 = C / q;        //ƽ���ʽ�Ƶ�һ�£�Ҳ��

    if(*x1 > *x2)
        std::swap(*x1, *x2);
    
    return true;
}