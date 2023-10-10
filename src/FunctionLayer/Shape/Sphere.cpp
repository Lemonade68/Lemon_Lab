#include"Sphere.h"

Sphere::Sphere(Point3f o, float r, const Vector3f &translate, const Vector3f &scale):Shape(translate, scale){
    center = o;
    radius = r;

    center = transform.toWorld(center);     //变换到世界坐标
    //TODO：包围盒操作
}

bool Sphere::rayIntersectShape(Ray &ray, Intersection &intersection) const{
    // Point3f origin = ray.origin;
    // Vector3f direction = normalize(ray.direction);
    // Vector3f o2c = center - origin;
    // float b = dot(o2c, direction);
    // float c = o2c.length() * o2c.length() - radius * radius;
    // float delta = b * b - c;

    // if(delta <= 0)
    //     return false;   //不相交

    // float sqrtDelta = std::sqrt(delta);
    // float t1 = b - sqrtDelta;
    // float t2 = b + sqrtDelta;

    // bool hit = false;
    // //先判断t2，如果后面t1也可以的话会直接替代t2（t1比t2小）
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
    //     intersection.normal = normal;               //交点处法线
    //     intersection.t = ray.tFar;                  //交点处对应t
    //     intersection.position = ray.at(ray.tFar);   //交点位置
    //     intersection.shape = this;                  //指向该物体

    //     //TODO计算纹理坐标
    //     get_sphere_uv(normal, intersection.texCoord);   //再看对不对
    // }
    // return hit;


    //待查看上面的问题在哪里

    Vector3f oc = ray.origin - center;
	auto a = ray.direction.squared_length();	//a大于0
	auto half_b = dot(oc, ray.direction);
	auto c = oc.squared_length() - radius * radius;
	auto discriminant = half_b * half_b - a * c;


    if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		//如果近处交点能被光线（射线）射中：直接记录近处交点信息
		if (temp < ray.tFar && temp > ray.tNear) {
            ray.tFar = temp;        //更新光线信息
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            intersection.normal = normal;               //交点处法线
            intersection.t = temp;                      //交点处对应t
            intersection.position = ray.at(ray.tFar);   //交点位置
            intersection.shape = this;                  //指向该物体

            //TODO计算纹理坐标
            get_sphere_uv(normal, intersection.texCoord);   //再看对不对
			return true;
		}
        
		temp = (-half_b + root) / a;
		if (temp < ray.tFar && temp > ray.tNear) {
			ray.tFar = temp;        //更新光线信息
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            intersection.normal = normal;               //交点处法线
            intersection.t = temp;                      //交点处对应t
            intersection.position = ray.at(ray.tFar);   //交点位置
            intersection.shape = this;                  //指向该物体

            //TODO计算纹理坐标
            get_sphere_uv(normal, intersection.texCoord);   //再看对不对
			return true;
		}
	}
	return false;
}


void Sphere::get_sphere_uv(const Vector3f &normal, Vector2f &texCoord) const{
    auto phi = atan2(normal.x(), normal.z());   //坐标轴：x朝右，z朝前，y朝上   phi范围：-pi - pi
	auto theta = acos(normal.y());				//theta范围：0 - pi
	//将uv投影到[0,1]的区间上
	texCoord.x = (phi + PI) / PI * .5;		    //先变到0到2pi上，再除以2pi	
	texCoord.y = theta / PI;
}
