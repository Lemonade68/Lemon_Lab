#include"Sphere.h"

Sphere::Sphere(Point3f o, float r, const std::shared_ptr<Material> &_material, const std::shared_ptr<Light> &_light, const Vector3f &translate, const Vector3f &scale)
       : Shape(_material, _light, translate, scale){
    center = o;
    radius = r;

    center = transform.toWorld(center);     //变换到世界坐标
    //TODO：包围盒操作
    //三条半径构成斜对角向量
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
    //     *primID = 0;
    //     Vector3f normal = normalize(ray.at(ray.tFar) - center);
    //     //计算重心坐标（phi，theta）
    //     get_sphere_uv(normal, u, v);   //再看对不对
    // }
    // return hit;


    //待查看上面的问题在哪里===================================================
    //没问题(之前是Vector3f的length函数调用了glm库的length长度返回成向量的元素个数了)

    Vector3f oc = ray.origin - center;
	auto a = ray.direction.squared_length();	//a大于0
	auto half_b = dot(oc, ray.direction);
	auto c = oc.squared_length() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

    //欠考虑等于0的情况？相切的情况 (浮点数精确等于0的情况约等于没有)
    //添加=后whittedIntegrator中mirror可能出现的死循环问题暂时解决？  ——   没有解决，后面再说吧
    if (discriminant >= 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		//如果近处交点能被光线（射线）射中：直接记录近处交点信息
		if (temp < ray.tFar && temp > ray.tNear) {
            ray.tFar = temp;        //更新光线信息
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            get_sphere_uv(normal, u, v);            //计算重心坐标（其实就是phi和theta）
            *primID = 0;
            return true;
		}
        
		temp = (-half_b + root) / a;
		if (temp < ray.tFar && temp > ray.tNear) {
			ray.tFar = temp;        //更新光线信息
            Vector3f normal = normalize(ray.at(ray.tFar) - center);
            get_sphere_uv(normal, u, v);
            *primID = 0;
            return true;
		}
	}
	return false;
}


//其中，u就是phi，v就是theta
void Sphere::fillIntersection(float tFar, int primID, float u, float v, Intersection *intersection) const {
    intersection->shape = this;
    intersection->t = tFar;
    Vector3f normal = Vector3f{std::sin(v) * std::sin(u), std::cos(v), std::sin(v) * std::cos(u)};
    intersection->normal = normal;
    intersection->position = center + radius * normal;
    intersection->texCoord = Vector2f{u * INV_PI * .5f, v * INV_PI};    //将phi和theta变为[0,1]的范围
    
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



//计算重心坐标
void Sphere::get_sphere_uv(const Vector3f &normal, float *u, float *v) const{
    auto phi = atan2(normal.x(), normal.z());   //坐标轴：x朝右，z朝前，y朝上   phi范围：-pi - pi
	auto theta = acos(normal.y());				//theta范围：0 - pi

    //记录u/v，其实就是phi和theta(?)
    *u = phi;
    *v = theta;
}
