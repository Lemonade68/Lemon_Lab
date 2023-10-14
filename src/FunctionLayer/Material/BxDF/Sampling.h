#pragma once
#include"CoreLayer/Math/Math.h"

//直接copy的Moer-lite上的代码
//再思考pdf为啥要判断y轴方向是否大于0

//半球均匀采样
inline Vector3f squareToUniformHemisphere(Vector2f sample) {
  float y = 1 - 2 * sample[0];
  float r = std::sqrt(std::max(.0f, 1.f - y * y));
  float phi = 2 * PI * sample[1];
  Vector3f dir{r * std::sin(phi), std::abs(y), r * std::cos(phi)};
  return normalize(dir);


  // //网址：https://zhuanlan.zhihu.com/p/360420413
  // float x = std::cos(2 * PI * sample.y) * std::sqrt(1 - (1 - sample.x) * (1 - sample.x));
  // float y = std::sin(2 * PI * sample.y) * std::sqrt(1 - (1 - sample.x) * (1 - sample.x));
  // float z = 1 - sample.x;

  // Vector3f dir{x, y, z};
  // return normalize(dir);
}

//返回半球均匀采样pdf (1/2π)
inline float squareToUniformHemispherePdf(Vector3f v) {
  return v[1] >= .0f ? INV_PI * .5f : .0f;
}

//=============================================================================

//半球Cosine权重采样   ——   再看
inline Vector3f squareToCosineHemisphere(Vector2f sample) {
  float phi = 2 * PI * sample[0], theta = std::acos(std::sqrt(sample[1]));
  return Vector3f{std::sin(theta) * std::sin(phi), std::cos(theta),
                  std::sin(theta) * std::cos(phi)};


  // //网址：https://zhuanlan.zhihu.com/p/360420413
  // float x = std::cos(2 * PI * sample.y) * std::sqrt(sample.x);
  // float y = std::sin(2 * PI * sample.y) * std::sqrt(sample.x);
  // float z = std::sqrt(1 - sample.x);

  // Vector3f dir{x, y, z};
  // return normalize(dir);
}

//返回半球Cosine采样的pdf
inline float squareToCosineHemispherePdf(Vector3f v) {
  // return (v[1] > .0f) ? v[1] * std::sqrt(1 - v[1] * v[1]) * INV_PI : .0f;     //cosθ * sinθ / π ？
  return (v[1] > .0f) ? v[1] * INV_PI : .0f;     
}