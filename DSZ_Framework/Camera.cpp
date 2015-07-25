#include "Camera.h"
#include "Globals.h"
#include "Game.h"

using namespace glm;

Camera::Camera(float fovy, float aspectRation, float nearPlane, float farPlane, float initialViewDistance, vec3 target, float initialRotation, float initialElevation, vec3 up)
:target(target), up(up), needViewResync(true), reference(0, 0, -1, 1), viewDistance(initialViewDistance), rotation(initialRotation), elevation(initialElevation)
{
	projectionMatrix = perspective(fovy, aspectRation, nearPlane, farPlane);
}

const mat4x4& Camera::GetProjection() const
{
	return projectionMatrix;
}

const mat4x4& Camera::GetViewMatrix()
{
	if (needViewResync)
	{
		mat4x4 m;
		m *= rotate(elevation, vec3(1, 0, 0));
		m *= rotate(rotation, vec3(0, 1, 0));

		position = vec3(reference * m);
		position *= viewDistance;
		position += target;

		CreateLookAt(position, target, up);

		needViewResync = false;
	}

	return viewMatrix;
}

vec3 Camera::GetPosition()
{
	return position;
}

void Camera::CreateLookAt(vec3 pos, vec3 target, vec3 up)
{
	vec3 z = -glm::normalize(target - pos);
	vec3 x = glm::normalize(glm::cross(up, z));
	vec3 y = glm::normalize(glm::cross(z, x));

	mat4x4 viewMatrix;

	vec3 poss = vec3(glm::dot(x, pos), glm::dot(y, pos), glm::dot(z, pos));
	viewMatrix *= glm::translate(vec3(-poss));

	viewMatrix[0][0] = x[0];
	viewMatrix[1][0] = x[1];
	viewMatrix[2][0] = x[2];

	viewMatrix[0][1] = y[0];
	viewMatrix[1][1] = y[1];
	viewMatrix[2][1] = y[2];

	viewMatrix[0][2] = z[0];
	viewMatrix[1][2] = z[1];
	viewMatrix[2][2] = z[2];

	this->viewMatrix = viewMatrix;
}

vec3 getForward(const mat4x4 &m)
{
	return vec3(-m[0][2], -m[1][2], -m[2][2]);
}

vec3 getLeft(const mat4x4 &m)
{
	return vec3(-m[0][0], -m[1][0], -m[2][0]);
}

vec3 getUp(const mat4x4 &m)
{
	return vec3(m[0][1], m[1][1], m[2][1]);
}

void Camera::Update(GameTime gameTime)
{
	if (Input::GetButtonState(MIDDLE) == DOWN)
	{
		rotation += glm::radians(Input::GetMouseCoordinates().x - Input::GetPreviouseMouseCoordinates().x);
		elevation -= glm::radians(Input::GetMouseCoordinates().y - Input::GetPreviouseMouseCoordinates().y);

		if (degrees(elevation) >= -10)
			elevation = radians(-10.0f);

		if (degrees(elevation) <= -89.9f)
			elevation = radians(-89.9f);

		needViewResync = true;
	}

	/*if (Input::GetButtonState(MIDDLE) == DOWN)
	{
		target += (Input::GetMouseCoordinates().x - Input::GetPreviouseMouseCoordinates().x) / sensitivity * getLeft(GetViewMatrix());
		target += (Input::GetMouseCoordinates().y - Input::GetPreviouseMouseCoordinates().y) / sensitivity * getUp(GetViewMatrix());;

		needViewResync = true;
	}*/

	if (Input::GetScrollValue() != 0)
	{
		viewDistance -= Input::GetScrollValue() / sensitivity;
		needViewResync = true;

		if (viewDistance <= 0.5f)
			viewDistance = 0.5f;
	}
}

vec3 Camera::Unproject(vec3 point)
{
	float x = (2.0f * point.x) / game->Width() - 1;
	float y = 1 - (2.0f * point.y) / game->Height();
	float z = point.z;

	vec4 ray_clip = vec4(x, y, z, 1.0f);

	vec4 ray_eye = glm::inverse(GetProjection()) * ray_clip;

	vec4 ray_world = glm::inverse(GetViewMatrix()) * ray_eye;

	ray_world /= ray_world.w;

	return vec3(ray_world);
}

void Camera::SetTarget(glm::vec3 target)
{
	this->target = target;
	needViewResync = true;
}