#pragma once

#include<iostream>
#include<algorithm>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//空间向量：对glm库进行一个封装
struct Vector3f{
    Vector3f(float f = .0f) { xyz = glm::vec3(.0f); }   //充当默认构造函数
    Vector3f(float x, float y, float z) : xyz(x, y, z) {}

public:
    //加法重载
    Vector3f operator+(const Vector3f& rhs) const { return xyz + rhs.xyz; }
    Vector3f &operator+=(const Vector3f& rhs) {
        xyz += rhs.xyz;
        return *this;
    }

    //减法重载
    Vector3f operator-(const Vector3f& rhs) const { return xyz - rhs.xyz; }
    Vector3f &operator-=(const Vector3f& rhs) {
        xyz -= rhs.xyz;
        return *this;
    }

    //与标量相乘
    Vector3f operator*(float f) const { return xyz * f; }
    Vector3f &operator*=(float f){
        xyz *= f;
        return *this;
    }

    //与标量相除
    Vector3f operator/(float f) const { return xyz / f; }
    Vector3f &operator/=(float f){
        xyz /= f;
        return *this;
    }

    //按位相乘（待考虑为什么要写）
    //按位相除同

    //访问xyz
    float operator[](int i) const { return xyz[i]; }    //这里要用const来区分
    float &operator[](int i) { return xyz[i]; }

    float x() const { return xyz.x; }
    float y() const { return xyz.y; }
    float z() const { return xyz.z; }
    float &x() { return xyz.x; }
    float &y() { return xyz.y; }
    float &z() { return xyz.z; }

    //返回向量长度
    float length() const { return xyz.length(); }

    //打印向量的值
    void debugPrint() const {
        printf("[vector<%d>](", 3);
        for (int i = 0; i < 3; ++i) 
            std::cout << (i == 0 ? '\0' : ',') << xyz[i];
        printf(")%c", '\n');
        fflush(stdout);
    }

//友函数声明
public:
    friend float dot(const Vector3f &v1, const Vector3f &v2);
    friend Vector3f cross(const Vector3f &v1, const Vector3f &v2);
    friend Vector3f normalize(const Vector3f &v);
    friend class Point3f;

private:
    Vector3f(glm::vec3 Vec3) : xyz(Vec3) { }  //从glm::vec3来生成Vector3f
    glm::vec3 xyz;      //针对这个进行封装
};

inline float dot(const Vector3f &v1, const Vector3f &v2){
    return glm::dot(v1.xyz, v2.xyz);
}

inline Vector3f cross(const Vector3f &v1, const Vector3f &v2){
    return glm::cross(v1.xyz, v2.xyz);
}

//向量取负号：
inline Vector3f operator-(const Vector3f &v) { return Vector3f() - v; }

//向量单位化
inline Vector3f normalize(const Vector3f &v) { return glm::normalize(v.xyz); }

//标量*向量
inline Vector3f operator*(float f, const Vector3f &v) { return v * f; }


//============================================================================

// 空间的点
struct Point3f
{
    Point3f(float f = .0f) { xyz = glm::vec3(.0f); }   //充当默认构造函数
    Point3f(float x, float y, float z) : xyz(x, y, z) {}

public:
    //加向量
    Point3f operator+(const Vector3f &rhs) const { return xyz + rhs.xyz; }
    Point3f &operator+=(const Vector3f &rhs){
        xyz += rhs.xyz;
        return *this;
    }

    //减向量
    Point3f operator-(const Vector3f &rhs) const { return xyz - rhs.xyz; }
    Point3f &operator-=(const Vector3f &rhs){
        xyz -= rhs.xyz;
        return *this;
    }

    //减点(不需要-=)
    Vector3f operator-(const Point3f &rhs) const { return xyz - rhs.xyz; }

    //访问xyz
    float operator[](int i) const { return xyz[i]; }    //这里要用const来区分
    float &operator[](int i) { return xyz[i]; }

    float x() const { return xyz.x; }
    float y() const { return xyz.y; }
    float z() const { return xyz.z; }
    float &x() { return xyz.x; }
    float &y() { return xyz.y; }
    float &z() { return xyz.z; }

    //打印点的值
    void debugPrint() const {
        printf("[point<%d>](", 3);
        for (int i = 0; i < 3; ++i) 
            std::cout << (i == 0 ? '\0' : ',') << xyz[i];
        printf(")%c", '\n');
        fflush(stdout);
    }

//友函数声明
public:
    friend class Vector3f;

private:
    Point3f(glm::vec3 Vec3) : xyz(Vec3) { }  //从glm::vec3来生成Vector3f
    glm::vec3 xyz;      //针对这个进行封装
};

using Matrix4f = glm::mat4x4;
using Vector2f = glm::vec2;
using Vector2i = glm::vec2;     //暂时存疑
using Vector4f = glm::vec4;

// using namespace glm;