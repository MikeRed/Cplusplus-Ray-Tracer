#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES
#include "ray.h"
#include <math.h>


/* Return random point within unit disk around lookfrom */
vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0f*vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera {
public:

	/*MEMBER VARIABLES*/
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float time0, time1; //Shutter open and close times
	float lens_radius;

	/*CONSTRUCTOR*/
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperature, float focus_dist, float t0, float t1) {
		time0 = t0;
		time1 = t1;
		lens_radius = aperature / 2;			//size of camera opening divided by 2
		float theta = vfov*M_PI / 180;			//vertical field of view conversion from degrees to radians
		float half_height = tan(theta / 2);		//half height of fov at any given z
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));			//vup is an upvector for the camera. The y-axis.
		v = cross(w, u);						//u, v, w describe camera orientation (form orthonormal basis for camera)
		lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
		horizontal = 2 * half_width*focus_dist*u;
		vertical = 2 * half_height*focus_dist*v;
	}

	ray get_ray(float s, float t) {
		vec3 rd = lens_radius*random_in_unit_disk(); //calculate random origin point of ray
		vec3 offset = u*rd.x() + v*rd.y();			//distance from origin point of ray
		float time = time0 + (rand() / (RAND_MAX + 1.0))*(time1 - time0); //Set time to random value between t0 and t1
		return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
	}
};

#endif

