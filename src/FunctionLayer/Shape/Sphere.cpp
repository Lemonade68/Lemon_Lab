#include"Sphere.h"

Sphere::Sphere(Point3f o, float r, const Vector3f &translate, const Vector3f &scale):Shape(translate, scale){
    center = o;
    radius = r;

    center = transform.toWorld(center);     //�任����������
    //TODO����Χ�в���
}

bool Sphere::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    // Point3f origin = ray.origin;
    // Vector3f direction = normalize(ray.direction);
    // Vector3f o2c = center - origin;
    // float b = dot(o2c, direction);
    // float c = o2c.length() * o2c.length() - radius * radius;
    // float delta = b * b - c;

    // if(delta <= 0)
    //     return false;   //���ཻ

    // float sqrtDelta = std::sqrt(delta);
    // float t1 = b - sqrtDelta;
    // float t2 = b + sqrtDelta;

    // bool hit = false;
    // //���ж�t2���������t1Ҳ���ԵĻ���ֱ�����t2��t1��t2С��
    // if (ray.tNear <= t2 && t2 <= ray.tFar) {
    //     ray.tFar = t2;
    //     hit = true;
    // }
    // if (ray.tNear <= t1 && t1 <= ray.tFar) {
    //     ray.tFar = t1;
    //     hit = true;
    // }

    // if(hit){
    //     Vector3f normal = normalize(ray.at(ray.tFar) - center);
    //     intersection.normal = normal;               //���㴦����
    //     intersection.t = ray.tFar;                  //���㴦��Ӧt
    //     intersection.position = ray.at(ray.tFar);   //����λ��
    //     intersection.shape = this;                  //ָ�������

    //     //TODO������������
    //     get_sphere_uv(normal, intersection.texCoord);   //�ٿ��Բ���
    // }
    // return hit;


    //���鿴���������������

    Vector3f oc = ray.origin - center;
	auto a = ray.direction.squared_length();	//a����0
	auto half_b = dot(oc, ray.direction);
	auto c = oc.squared_length() - radius * radius;
	auto discriminant = half_b * half_b - a * c;


    if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		//������������ܱ����ߣ����ߣ����У�ֱ�Ӽ�¼����������Ϣ
		if (temp < ray.tFar && temp > ray.tNear) {
            ray.tFar = temp;        //���¹�����Ϣ
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            intersection.normal = normal;               //���㴦����
            intersection.t = temp;                      //���㴦��Ӧt
            intersection.position = ray.at(ray.tFar);   //����λ��
            intersection.shape = this;                  //ָ�������

            //TODO������������
            get_sphere_uv(normal, intersection.texCoord);   //�ٿ��Բ���
			return true;
		}
        
		temp = (-half_b + root) / a;
		if (temp < ray.tFar && temp > ray.tNear) {
			ray.tFar = temp;        //���¹�����Ϣ
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            intersection.normal = normal;               //���㴦����
            intersection.t = temp;                      //���㴦��Ӧt
            intersection.position = ray.at(ray.tFar);   //����λ��
            intersection.shape = this;                  //ָ�������

            //TODO������������
            get_sphere_uv(normal, intersection.texCoord);   //�ٿ��Բ���
			return true;
		}
	}
	return false;
}


void Sphere::get_sphere_uv(const Vector3f &normal, Vector2f &texCoord) const{
    auto phi = atan2(normal.x(), normal.z());   //�����᣺x���ң�z��ǰ��y����   phi��Χ��-pi - pi
	auto theta = acos(normal.y());				//theta��Χ��0 - pi
	//��uvͶӰ��[0,1]��������
	texCoord.x = (phi + PI) / PI * .5;		    //�ȱ䵽0��2pi�ϣ��ٳ���2pi	
	texCoord.y = theta / PI;
}
