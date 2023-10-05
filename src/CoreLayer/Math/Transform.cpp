#include"Transform.h"

Matrix4f Transform::translation(const Vector3f &offset){
    Matrix4f mat(1.f);      //��λ����
    for (int i = 0; i < 3; ++i)
        mat[i][3] = offset[i];
    return mat;
}

//�������ԭ��ɼ���https://zhuanlan.zhihu.com/p/462935097?utm_id=0
Matrix4f Transform::rotation(const Vector3f &axis, float radian){
    Vector3f a = normalize(axis);
    float sin_theta = std::sin(radian), cos_theta = std::cos(radian);
    Matrix4f mat(1.f);

    mat[0][0] = cos_theta + (1 - cos_theta) * a[0] * a[0];
    mat[1][0] = (1 - cos_theta) * a[0] * a[1] + sin_theta * a[2];
    mat[2][0] = (1 - cos_theta) * a[0] * a[2] - sin_theta * a[1];
    mat[3][0] = 0;

    mat[0][1] = (1 - cos_theta) * a[0] * a[1] - sin_theta * a[2];
    mat[1][1] = cos_theta + (1 - cos_theta) * a[1] * a[1];
    mat[2][1] = (1 - cos_theta) * a[1] * a[2] + sin_theta * a[0];
    mat[3][1] = 0;

    mat[0][2] = (1 - cos_theta) * a[0] * a[2] + sin_theta * a[1];
    mat[1][2] = (1 - cos_theta) * a[1] * a[2] - sin_theta * a[0];
    mat[2][2] = cos_theta + (1 - cos_theta) * a[2] * a[2];
    mat[3][2] = 0;

    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = 0;
    mat[3][3] = 1;

    return mat;
}

Matrix4f Transform::scalation(const Vector3f &scale){
    Matrix4f mat(1.f);
    for (int i = 0; i < 3; ++i)
        mat[i][i] = scale[i];
    return mat;
}

Transform::Transform(){
    translate = invTranslate = rotate = invRotate = scale = invScale = Matrix4f(1.f);
}

Transform::Transform(const Matrix4f &_translation, const Matrix4f &_rotation, const Matrix4f &_scalation){
    translate = _translation;
    rotate = _rotation;
    scale = _scalation;
    invTranslate = invRotate = invScale = Matrix4f(1.f);
    for (int i = 0; i < 3; ++i){
        invTranslate[i][3] = -translate[i][3];      //ƽ��Ϊ��
        invScale[i][i] = 1.f / rotate[i][i];        //����Ϊ�෴�ı���
    }
    invRotate = glm::transpose(rotate);                  //��ת��������������ת��
}

Vector3f Transform::toWorld(const Vector3f &vector) const{
    Vector4f v4(vector[0], vector[1], vector[2], 0.f);  //������������ʾ
    v4 = translate * rotate * scale * v4;
    return Vector3f(v4[0], v4[1], v4[2]);
}

Point3f Transform::toWorld(const Point3f &point) const{
    Vector4f v4(point[0], point[1], point[2], 1.f);     //�����������ʾ
    v4 = translate * rotate * scale * v4;
    v4 /= v4[3];
    return Point3f(v4[0], v4[1], v4[2]);
}