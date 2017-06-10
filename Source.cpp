#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "moving_sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "aabb.h"
#include "bvh.h"
#include "texture.h"
#include "perlin.h"
#include "surface_texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
* A ray is shot through each pixel in our 200x100 box. This function
* determines the color at each pixel that the rays touch.
*/
vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return emitted + attenuation*color(scattered, world, depth + 1);
		else
			return emitted;
	}
	else
		return vec3(0, 0, 0);
}


/*
* NOTE: If running the earth() function, use this version of color().
*
vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		//return color result from LERP of white to blue, depending on y position on canvas.
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}
*/

hitable *cornell_smoke() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
	return new hitable_list(list, i);
}

hitable *cornell_box() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *simple_light() {
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable *earth() { // Assign an image to a diffuse material
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earth-living.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny));
	hitable **list = new hitable*[1];
	list[0] = new sphere(vec3(0, 0, 0), 2, mat);
	return new hitable_list(list, 1);
}

hitable *two_perlin_spheres() {
	texture *pertext = new noise_texture(5);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new hitable_list(list, 2);
}

hitable *random_scene() {
	int n = 50000;
	hitable **list = new hitable*[n + 1];
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = 0; a < 1; a++) {
		for (int b = 0; b < 2; b++) {
			float choose_mat = rand() / (RAND_MAX + 1.0);
			vec3 center(a + 0.9* rand() / (RAND_MAX + 1.0), 0.2, b + 0.9*rand() / (RAND_MAX + 1.0));
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) { //diffuse
					list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * rand() / (RAND_MAX + 1.0), 0), 0.0, 1.0, 0.2,
						new lambertian(new constant_texture(vec3(rand() / (RAND_MAX + 1.0) * rand() / (RAND_MAX + 1.0),
							rand() / (RAND_MAX + 1.0)*rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)*rand() / (RAND_MAX + 1.0)))));
				}
				else if (choose_mat < 0.95) { //metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + rand() / (RAND_MAX + 1.0)), 0.5*(1 + rand() / (RAND_MAX + 1.0)), 0.5*(1 + rand() / (RAND_MAX + 1.0))), 0.5*rand() / (RAND_MAX + 1.0)));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

hitable *moving_sphere_example() {
	hitable **list = new hitable*[2];
	texture *c1 = new constant_texture(vec3(0.6, 0.6, 0.6));
	texture *c2 = new constant_texture(vec3(0.8, 0.8, 0.8));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(c1));
	list[1] = new moving_sphere(vec3(0, 1, 0), vec3(0,4,0), 0.0, 1.0, 1.0, new lambertian(c2));
	return new hitable_list(list, 2);
}

hitable *a1example() {
	hitable *list[4]; //Array of hitables
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.9)))); //Position (center, one unit towards camera, radius, material (color))
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	return new hitable_list(list, 4);
}

int main() {
	//OUTPUT 200*100 PIXEL COLOUR CODES (PPM FORMAT)
	int nx = 200; //number of x coordinate pixels
	int ny = 100; //number of y coorinate pixels
	int ns = 200; //number of samples taken from each pixel
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	//hitable *world = a1example();
	//hitable *world = random_scene();
	//hitable *world = two_perlin_spheres();
	//hitable *world = earth();
	//hitable *world = simple_light();
	hitable *world = cornell_box();
	//hitable *world = cornell_smoke();

	//vec3 lookfrom(13, 2, 3); //Use these parameters when using a1example, random_scene, two_perlin_spheres, or earth
	//vec3 lookat(0, 1, 0);

	vec3 lookfrom(278, 278, -800); //origin point of camera. x = rotate around y axis, y = rotate around x axis, z = zoom
	vec3 lookat(278, 278, 0);
	
	float dist_to_focus = 10.0;
	float aperature = 0.0; //Size of camera hole
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 40, float(nx) / float(ny), aperature, dist_to_focus, 0.0, 1.0);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) { //For every pixel in our 200x100 window
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { //For the number of samples taken from each pixel
				float u = float(i + rand() / (RAND_MAX + 1.0)) / float(nx);
				float v = float(j + rand() / (RAND_MAX + 1.0)) / float(ny);
				ray r = cam.get_ray(u, v); //Create a ray coming from the camera lens
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0); // Check if it hits an object and return the object's color if it does.
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}