#include"Transform.h"

Matrix4f Transform::translation(const Vector3f &offset){
    Matrix4f mat(1.f);      //��λ����
    for (int i = 0; i < 3; ++i)
        mat[i][3] = offset[i];

    mat = transpose(mat);       //glm��ľ�����������
    
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

    mat = transpose(mat);       //glm��ľ�����������
    
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
        invTranslate[3][i] = -translate[3][i];      //ƽ��Ϊ����������
        invScale[i][i] = 1.f / rotate[i][i];        //����Ϊ�෴�ı���
    }
    invRotate = transpose(rotate);                  //��ת��������������ת��
}

Vector3f Transform::toWorld(const Vector3f &vector) const{
    Vector4f v4(vector[0], vector[1], vector[2], 0.f);  //������������ʾ
    v4 = translate * rotate * scale * v4;
    return Vector3f(v4[0], v4[1], v4[2]);
}

Point3f Transform::toWorld(const Point3f &point) const{
    glm::vec4 v4(point[0], point[1], point[2], 1.0f);     //�����������ʾ
    // ����debug��
    v4 = translate * rotate * scale * v4;
    v4 /= v4[3];                                    //������44�������vec4�ļ������glm��������
    return Point3f(v4[0], v4[1], v4[2]);
}

Ray Transform::RayToLocal(const Ray &ray) const{
    Point3f origin = ray.origin;
    Vector3f direction = ray.direction;

    Vector4f o(origin.x(), origin.y(), origin.z(), 1.f);
    Vector4f d(direction.x(), direction.y(), direction.z(), 0.f);

    //����ռ䵽����ռ�����rotate��translate��������������invTranslate��invRoate
    //���߲�����scale
    o = invRotate * invTranslate * o;
    d = invRotate * invTranslate * d;
    o /= o.w;

    origin = Point3f(o.x, o.y, o.z);
    direction = Vector3f(d.x, d.y, d.z);
    return Ray(origin, direction, ray.tNear, ray.tFar);
}

AABB Transform::toWorld(const AABB &b) const {
    AABB ret;
    ret = ret.Union(toWorld(Point3f(b.pMin[0], b.pMin[1], b.pMin[2])));
    ret = ret.Union(toWorld(Point3f(b.pMax[0], b.pMin[1], b.pMin[2])));
    ret = ret.Union(toWorld(Point3f(b.pMin[0], b.pMax[1], b.pMin[2])));
    ret = ret.Union(toWorld(Point3f(b.pMin[0], b.pMin[1], b.pMax[2])));
    ret = ret.Union(toWorld(Point3f(b.pMin[0], b.pMax[1], b.pMax[2])));
    ret = ret.Union(toWorld(Point3f(b.pMax[0], b.pMax[1], b.pMin[2])));
    ret = ret.Union(toWorld(Point3f(b.pMax[0], b.pMin[1], b.pMax[2])));
    ret = ret.Union(toWorld(Point3f(b.pMax[0], b.pMax[1], b.pMax[2])));
    return ret;
}