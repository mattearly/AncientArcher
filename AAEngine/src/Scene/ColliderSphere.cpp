#include "../Scene/ColliderSphere.h"

ColliderSphere::ColliderSphere(glm::vec3 loc, float r) :
	center(loc), radius(r)
{}

ColliderSphere::ColliderSphere()
{
	center = glm::vec3(0);
	radius = 1.f;
}