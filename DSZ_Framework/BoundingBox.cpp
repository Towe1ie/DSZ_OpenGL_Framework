#include "BoundingBox.h"
#include <iostream>

using namespace glm;

BoundingBox::BoundingBox(vec3 p1, vec3 p2)
:p1(p1), p2(p2)
{
	vertices = new vec3[8];

	vertices[0] = p1;
	vertices[1] = vec3(p2.x, p1.y, p1.z);
	vertices[2] = vec3(p2.x, p2.y, p1.z);
	vertices[3] = vec3(p1.x, p2.y, p1.z);

	vertices[4] = vec3(p1.x, p1.y, p2.z);
	vertices[5] = vec3(p2.x, p1.y, p2.z);
	vertices[6] = p2;
	vertices[7] = vec3(p1.x, p2.y, p2.z);

	indices = new GLuint[24];

	for (GLuint i = 0; i < 8; i++)
	{
		indices[i * 2 + 0] = i;
		if (i == 3 || i == 7)
			indices[i * 2 + 1] = i - 3;
		else
			indices[i * 2 + 1] = i + 1;
	}

	for (GLuint i = 8; i < 12; i++)
	{
		indices[i * 2 + 0] = i - 8;
		indices[i * 2 + 1] = i - 4;
	}



	color = vec4(0, 1, 0, 1);
}

void BoundingBox::Draw()
{

}

BoundingBox::~BoundingBox()
{
	delete[] vertices;
	delete[] indices;

}

bool BoundingBox::IntersectsRay(glm::vec3 origin, glm::vec3 dir, float t0, float t1)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	
	vec3 direction = normalize(dir);

	if (direction.x >= 0) {
		tmin = (p1.x - origin.x) / direction.x;
		tmax = (p2.x - origin.x) / direction.x;
	}
	else {
		tmin = (p2.x - origin.x) / direction.x;
		tmax = (p1.x - origin.x) / direction.x;
	}
	if (direction.y >= 0) {
		tymin = (p1.y - origin.y) / direction.y;
		tymax = (p2.y - origin.y) / direction.y;
	}
	else {
		tymin = (p2.y - origin.y) / direction.y;
		tymax = (p1.y - origin.y) / direction.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (direction.z >= 0) {
		tzmin = (p1.z - origin.z) / direction.z;
		tzmax = (p2.z - origin.z) / direction.z;
	}
	else {
		tzmin = (p2.z - origin.z) / direction.z;
		tzmax = (p1.z - origin.z) / direction.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < t1) && (tmax > t0));
}