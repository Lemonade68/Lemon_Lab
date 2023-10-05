#pragma once

#include<iostream>
#include<algorithm>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//�ռ���������glm�����һ����װ
struct Vector3f{
    Vector3f(float f = .0f) { xyz = glm::vec3(.0f); }   //�䵱Ĭ�Ϲ��캯��
    Vector3f(float x, float y, float z) : xyz(x, y, z) {}

public:
    //�ӷ�����
    Vector3f operator+(const Vector3f& rhs) const { return xyz + rhs.xyz; }
    Vector3f &operator+=(const Vector3f& rhs) {
        xyz += rhs.xyz;
        return *this;
    }

    //��������
    Vector3f operator-(const Vector3f& rhs) const { return xyz - rhs.xyz; }
    Vector3f &operator-=(const Vector3f& rhs) {
        xyz -= rhs.xyz;
        return *this;
    }

    //��������
    Vector3f operator*(float f) const { return xyz * f; }
    Vector3f &operator*=(float f){
        xyz *= f;
        return *this;
    }

    //��������
    Vector3f operator/(float f) const { return xyz / f; }
    Vector3f &operator/=(float f){
        xyz /= f;
        return *this;
    }

    //��λ��ˣ�������ΪʲôҪд��
    //��λ���ͬ

    //����xyz
    float operator[](int i) const { return xyz[i]; }    //����Ҫ��const������
    float &operator[](int i) { return xyz[i]; }

    float x() const { return xyz.x; }
    float y() const { return xyz.y; }
    float z() const { return xyz.z; }
    float &x() { return xyz.x; }
    float &y() { return xyz.y; }
    float &z() { return xyz.z; }

    //������������
    float length() const { return xyz.length(); }

    //��ӡ������ֵ
    void debugPrint() const {
        printf("[vector<%d>](", 3);
        for (int i = 0; i < 3; ++i) 
            std::cout << (i == 0 ? '\0' : ',') << xyz[i];
        printf(")%c", '\n');
        fflush(stdout);
    }

//�Ѻ�������
public:
    friend float dot(const Vector3f &v1, const Vector3f &v2);
    friend Vector3f cross(const Vector3f &v1, const Vector3f &v2);
    friend Vector3f normalize(const Vector3f &v);
    friend class Point3f;

private:
    Vector3f(glm::vec3 Vec3) : xyz(Vec3) { }  //��glm::vec3������Vector3f
    glm::vec3 xyz;      //���������з�װ
};

inline float dot(const Vector3f &v1, const Vector3f &v2){
    return glm::dot(v1.xyz, v2.xyz);
}

inline Vector3f cross(const Vector3f &v1, const Vector3f &v2){
    return glm::cross(v1.xyz, v2.xyz);
}

//����ȡ���ţ�
inline Vector3f operator-(const Vector3f &v) { return Vector3f() - v; }

//������λ��
inline Vector3f normalize(const Vector3f &v) { return glm::normalize(v.xyz); }

//����*����
inline Vector3f operator*(float f, const Vector3f &v) { return v * f; }


//============================================================================

// �ռ�ĵ�
struct Point3f
{
    Point3f(float f = .0f) { xyz = glm::vec3(.0f); }   //�䵱Ĭ�Ϲ��캯��
    Point3f(float x, float y, float z) : xyz(x, y, z) {}

public:
    //������
    Point3f operator+(const Vector3f &rhs) const { return xyz + rhs.xyz; }
    Point3f &operator+=(const Vector3f &rhs){
        xyz += rhs.xyz;
        return *this;
    }

    //������
    Point3f operator-(const Vector3f &rhs) const { return xyz - rhs.xyz; }
    Point3f &operator-=(const Vector3f &rhs){
        xyz -= rhs.xyz;
        return *this;
    }

    //����(����Ҫ-=)
    Vector3f operator-(const Point3f &rhs) const { return xyz - rhs.xyz; }

    //����xyz
    float operator[](int i) const { return xyz[i]; }    //����Ҫ��const������
    float &operator[](int i) { return xyz[i]; }

    float x() const { return xyz.x; }
    float y() const { return xyz.y; }
    float z() const { return xyz.z; }
    float &x() { return xyz.x; }
    float &y() { return xyz.y; }
    float &z() { return xyz.z; }

    //��ӡ���ֵ
    void debugPrint() const {
        printf("[point<%d>](", 3);
        for (int i = 0; i < 3; ++i) 
            std::cout << (i == 0 ? '\0' : ',') << xyz[i];
        printf(")%c", '\n');
        fflush(stdout);
    }

//�Ѻ�������
public:
    friend class Vector3f;

private:
    Point3f(glm::vec3 Vec3) : xyz(Vec3) { }  //��glm::vec3������Vector3f
    glm::vec3 xyz;      //���������з�װ
};

using Matrix4f = glm::mat4x4;
using Vector2f = glm::vec2;
using Vector2i = glm::vec2;     //��ʱ����
using Vector4f = glm::vec4;

// using namespace glm;