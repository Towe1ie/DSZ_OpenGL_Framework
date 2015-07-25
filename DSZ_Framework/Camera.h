#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DSZMathHeader.h"
#include "DSZCore.h"

#define AR_4_3		(4.0/3.0)
#define AR_16_9		(16.0/9.0)
#define AR_16_10	(16.0/10.0)

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 target;
	float rotation;
	float elevation;
	float viewDistance;
	glm::vec4 reference;
	glm::vec3 up;

	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;

	bool needViewResync;

public:
	Camera(float fovy, float aspectRation, float nearPlane, float farPlane, float initialViewDistance = 10, glm::vec3 target = glm::vec3(0, 0, 0), float initialRotation = 0, float initialElevation = 0, glm::vec3 up = glm::vec3(0, 1, 0));

	const glm::mat4x4& GetProjection() const;
	const glm::mat4x4& GetViewMatrix();

	glm::vec3 GetPosition();
	void SetTarget(glm::vec3 target);

	void CreateLookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);



	glm::vec3 Unproject(glm::vec3 point);

	void Update(GameTime gameTime);

	float sensitivity = 100.0f;
};

#endif