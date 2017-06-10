#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	/*MEMBER VARIABLES*/
	vec3 center;
	float radius;
	material *mat_ptr;

	/*CONSTRUCTORS*/
	sphere() {}
	sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;					//vector from center to ray origin
	float a = dot(r.direction(), r.direction());	//length of rays directino vector
	float b = dot(oc, r.direction());				//length of projection of oc onto r.direction
	float c = dot(oc, oc) - radius*radius;			//length from ray origin to edge of sphere
	float discriminant = b*b - a*c;
	if (discriminant > 0) {							//two solutions to t -> sphere is hit twice
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {			//if one of the solutions to t falls within our range of acceptable t values
			rec.t = temp;							//Set hit record values and return true
			rec.p = r.point_at_parameter(rec.t);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {			//if the other solution to t falls within t range
			rec.t = temp;							//set hit record values and return true
			rec.p = r.point_at_parameter(rec.t);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	box = aabb(center - vec3(radius, radius, radius), 
		center + vec3(radius, radius, radius));
	return true;
}
#endif

