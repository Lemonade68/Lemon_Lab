#pragma once
#include"CoreLayer/Math/Math.h"

//ֱ��copy��Moer-lite�ϵĴ���
//��˼��pdfΪɶҪ�ж�y�᷽���Ƿ����0

//������Ȳ���
inline Vector3f squareToUniformHemisphere(Vector2f sample) {
  float y = 1 - 2 * sample[0];
  float r = std::sqrt(std::max(.0f, 1.f - y * y));
  float phi = 2 * PI * sample[1];
  Vector3f dir{r * std::sin(phi), std::abs(y), r * std::cos(phi)};
  return normalize(dir);


  // //��ַ��https://zhuanlan.zhihu.com/p/360420413
  // float x = std::cos(2 * PI * sample.y) * std::sqrt(1 - (1 - sample.x) * (1 - sample.x));
  // float y = std::sin(2 * PI * sample.y) * std::sqrt(1 - (1 - sample.x) * (1 - sample.x));
  // float z = 1 - sample.x;

  // Vector3f dir{x, y, z};
  // return normalize(dir);
}

//���ذ�����Ȳ���pdf (1/2��)
inline float squareToUniformHemispherePdf(Vector3f v) {
  return v[1] >= .0f ? INV_PI * .5f : .0f;
}

//=============================================================================

//����CosineȨ�ز���   ����   �ٿ�
inline Vector3f squareToCosineHemisphere(Vector2f sample) {
  float phi = 2 * PI * sample[0], theta = std::acos(std::sqrt(sample[1]));
  return Vector3f{std::sin(theta) * std::sin(phi), std::cos(theta),
                  std::sin(theta) * std::cos(phi)};


  // //��ַ��https://zhuanlan.zhihu.com/p/360420413
  // float x = std::cos(2 * PI * sample.y) * std::sqrt(sample.x);
  // float y = std::sin(2 * PI * sample.y) * std::sqrt(sample.x);
  // float z = std::sqrt(1 - sample.x);

  // Vector3f dir{x, y, z};
  // return normalize(dir);
}

//���ذ���Cosine������pdf
inline float squareToCosineHemispherePdf(Vector3f v) {
  // return (v[1] > .0f) ? v[1] * std::sqrt(1 - v[1] * v[1]) * INV_PI : .0f;     //cos�� * sin�� / �� ��
  return (v[1] > .0f) ? v[1] * INV_PI : .0f;     
}