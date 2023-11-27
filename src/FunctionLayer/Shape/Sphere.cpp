#include"Sphere.h"

Sphere::Sphere(Point3f o, float r, const std::shared_ptr<Material> &_material, const std::shared_ptr<Light> &_light, const Vector3f &translate, const Vector3f &scale)
       : Shape(_material, _light, translate, scale){
    center = o;
    radius = r;

    center = transform.toWorld(center);     //�任����������
    //TODO����Χ�в���
    //�����뾶����б�Խ�����
    boundingBox = AABB(center - Vector3f(radius), center + Vector3f(radius));
}

bool Sphere::rayIntersectShape(Ray &ray, int *primID, float *u, float *v) const{
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
    //     *primID = 0;
    //     Vector3f normal = normalize(ray.at(ray.tFar) - center);
    //     //�����������꣨phi��theta��
    //     get_sphere_uv(normal, u, v);   //�ٿ��Բ���
    // }
    // return hit;


    //���鿴���������������===================================================
    //û����(֮ǰ��Vector3f��length����������glm���length���ȷ��س�������Ԫ�ظ�����)

    Vector3f oc = ray.origin - center;
	auto a = ray.direction.squared_length();	//a����0
	auto half_b = dot(oc, ray.direction);
	auto c = oc.squared_length() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

    //Ƿ���ǵ���0����������е���� (��������ȷ����0�����Լ����û��)
    //���=��whittedIntegrator��mirror���ܳ��ֵ���ѭ��������ʱ�����  ����   û�н����������˵��
    if (discriminant >= 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		//������������ܱ����ߣ����ߣ����У�ֱ�Ӽ�¼����������Ϣ
		if (temp < ray.tFar && temp > ray.tNear) {
            ray.tFar = temp;        //���¹�����Ϣ
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            get_sphere_uv(normal, u, v);            //�����������꣨��ʵ����phi��theta��
            *primID = 0;
            return true;
		}
        
		temp = (-half_b + root) / a;
		if (temp < ray.tFar && temp > ray.tNear) {
			ray.tFar = temp;        //���¹�����Ϣ
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            get_sphere_uv(normal, u, v);
            *primID = 0;
            return true;
		}
	}
	return false;
}


//���У�u����phi��v����theta
void Sphere::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const {
    intersection->shape = this;
    intersection->t = tFar;
    Vector3f normal = Vector3f{std::sin(v) * std::sin(u), std::cos(v), std::sin(v) * std::cos(u)};
    intersection->normal = normal;
    intersection->position = center + radius * normal;
    intersection->texCoord = Vector2f{u * INV_PI * .5f, v * INV_PI};    //��phi��theta��Ϊ[0,1]�ķ�Χ
    
    Vector3f tangent{1.f, 0.f, .0f};
    Vector3f bitangent;
    if (std::abs(dot(tangent, intersection->normal)) > .9f) {
        tangent = Vector3f(.0f, 1.f, .0f);
    }
    bitangent = normalize(cross(tangent, intersection->normal));
    tangent = normalize(cross(intersection->normal, bitangent));
    intersection->tangent = tangent;
    intersection->bitangent = bitangent;
}



//������������
void Sphere::get_sphere_uv(const Vector3f &normal, float *u, float *v) const{
    auto phi = atan2(normal.x(), normal.z());   //�����᣺x���ң�z��ǰ��y����   phi��Χ��-pi - pi
	auto theta = acos(normal.y());				//theta��Χ��0 - pi

    //��¼u/v����ʵ����phi��theta(?)
    *u = phi;
    *v = theta;
}
