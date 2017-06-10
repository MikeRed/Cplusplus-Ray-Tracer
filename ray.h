#ifndef RAYH
#define RAYH
#include "vec3.h"

/*
* A ray is composed of an origin point, a direction vector, and a scalar t that
* determines how far the direction vector goes (and whether it goes backwards or
* forwards). This class is an abstraction of this concept.
*/
class ray {
public:
	/*MEMBER VARIABLES*/
	vec3 A; //ORIGIN POINT
	vec3 B; //DIRECTION VECTOR
	float _time; //TIME RAY EXISTS AT

	/*CONSTRUCTORS*/
	ray() {}
	ray(const vec3& a, const vec3& b, float ti = 0.0) { A = a; B = b; _time = ti; }

	/*FUNCTIONS FOR RETURNING MEMBER VARIABLES*/
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float time() const { return _time; }

	/*RETURN POINT ON RAY AT SCALAR t */
	vec3 point_at_parameter(float t) const { return A + t*B; }
};

#endif

