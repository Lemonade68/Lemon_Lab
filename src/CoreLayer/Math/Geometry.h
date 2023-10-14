#pragma once

#include<iostream>
#include<algorithm>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//�ռ���������glm�����һ����װ
struct Vector3f{
    Vector3f(float f = .0f) { xyz = glm::vec3(f); }   //�䵱Ĭ�Ϲ��캯��
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

    //��λ��ˣ�ΪʲôҪд����ɫ����ǰ�λ��ˣ�
    Vector3f operator*(const Vector3f &rhs) const{
        return Vector3f(rhs[0] * xyz[0], rhs[1] * xyz[1], rhs[2] * xyz[2]);
    }

    Vector3f &operator*=(const Vector3f &rhs){
        xyz = Vector3f(rhs[0] * xyz[0], rhs[1] * xyz[1], rhs[2] * xyz[2]).xyz;
        return *this;
    }
    
    //��λ���ͬ
    Vector3f operator/(const Vector3f &rhs) const{
        return Vector3f(xyz[0] / rhs[0], xyz[1] / rhs[1], xyz[2] / rhs[2]);
    }

    Vector3f &operator/=(const Vector3f &rhs){
        xyz = Vector3f(xyz[0] / rhs[0], xyz[1] / rhs[1], xyz[2] / rhs[2]).xyz;
        return *this;
    }

    //�����о������⣬������ȼ���
    bool operator==(const Vector3f &rhs) const{
        return (abs(rhs.xyz[0] - xyz[0]) < 1e-4f) &&
               (abs(rhs.xyz[1] - xyz[1]) < 1e-4f) &&
               (abs(rhs.xyz[2] - xyz[2]) < 1e-4f);
    }


    //����xyz
    float operator[](int i) const { return xyz[i]; }    //����Ҫ��const������
    float &operator[](int i) { return xyz[i]; }

    float x() const { return xyz.x; }
    float y() const { return xyz.y; }
    float z() const { return xyz.z; }
    float &x() { return xyz.x; }
    float &y() { return xyz.y; }
    float &z() { return xyz.z; }

    //������������ά���϶���0���򷵻�true
    bool isZero() const {
        return (xyz[0] == 0.f) && (xyz[1] == 0.f) && (xyz[2] == 0.f);
    }

    //������������    ֱ��xyz.length�����⣡���length���ص���������Ԫ�ظ�����
    float length() const { return sqrt(xyz.x * xyz.x + xyz.y * xyz.y + xyz.z * xyz.z); }

    //��ӡ������ֵ
    void debugPrint() const {
        printf("[vector<%d>](", 3);
        for (int i = 0; i < 3; ++i) 
            std::cout << (i == 0 ? '\0' : ',') << xyz[i];
        printf(")%c", '\n');
        fflush(stdout);
    }

    float squared_length() const{
        return xyz.x * xyz.x + xyz.y * xyz.y + xyz.z * xyz.z;
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
    Point3f(float f = .0f) { xyz = glm::vec3(f); }   //�䵱Ĭ�Ϲ��캯��
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

    //�����о������⣬������ȼ���
    bool operator==(const Point3f &rhs) const{
        return (abs(rhs.xyz[0] - xyz[0]) < 1e-4f) &&
               (abs(rhs.xyz[1] - xyz[1]) < 1e-4f) &&
               (abs(rhs.xyz[2] - xyz[2]) < 1e-4f);
    }

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


//�㵽������ת��
inline Vector3f p2v(const Point3f &p){
    return Vector3f(p.x(), p.y(), p.z());
}

//���������ת��
inline Point3f v2p(const Vector3f &v){
    return Point3f(v[0], v[1], v[2]);
}


using Matrix4f = glm::mat4;
using Vector2f = glm::vec2;
using Vector2i = glm::vec2;     //��ʱ����
using Vector4f = glm::vec4;

using namespace glm;

//��������˳���ӡ����
inline void PrintMat(const Matrix4f &mat){
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            std::cout << mat[j][i] << ' ';      //glm��ľ�����������
        }
        std::cout << std::endl;
    }
}

inline void PrintVec4(const Vector4f &v4){
    std::cout <<"vec4: "<< v4[0] << ' ' << v4[1] << ' ' << v4[2] << ' ' << v4[3] << '\n';
}