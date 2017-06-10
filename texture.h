#ifndef TEXTUREH
#define TEXTUREH

#include "perlin.h"

class texture {
public: 
	//Return color of texture at position
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture {
public:
	/*MEMBER VARIABLES*/
	vec3 color;

	/* CONSTRUCTORS */
	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}

	//Return color of texture (position doesnt matter, color is same at all points)
	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}
};

class checker_texture : public texture {
public:

	/* MEMBER VARIABLES*/
	texture *odd;
	texture *even;

	/* CONSTRUCTORS */
	checker_texture() { }
	checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) { }

	//Return color in a checker pattern (Use sinusoids to keep the pattern consistent)
	virtual vec3 value(float u, float v, const vec3& p) const {
		float sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

};

class noise_texture : public texture {
public:

	/* MEMBER VARIABLES */
	perlin noise;
	float scale; //Scale used to increase the variance of grey colors 

	/* CONSTRUCTORS */
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}

	/*
	* Takes floats between 0 and 1 and creates grey colors 
	* Use sinusoids to create a pattern out of the noise
	*/
	virtual vec3 value(float u, float v, const vec3& p) const {
		return vec3(1, 1, 1)*0.5*(1 + sin(scale*p.x() + 5 * noise.turb(scale*p)));
	}
};

#endif