#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <glm/glm.hpp>
#include "DSZCore.h"

class BoundingBox
{
private:
	glm::vec3 p1, p2;
	glm::vec3 *vertices;
	GLuint *indices;
	
	

public:
	BoundingBox(glm::vec3 p1, glm::vec3 p2);

	glm::vec4 color;

	void Draw();

	bool IntersectsRay(glm::vec3 origin, glm::vec3 direction, float t0, float t1);

	~BoundingBox();

	friend void DrawBoundingBox(BoundingBox &box, glm::vec4 color);
};

#endif