#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace RenderLib
{
	Camera::Camera(FLOAT near, FLOAT far, FLOAT FOV)
		: nearPlane(near)
		, farPlane(far)
		, fov(FOV)
	{
		initializeProjectionMatrix();
	}

	Camera::~Camera()
	{

	}

	void Camera::initializeProjectionMatrix()
	{
		const FLOAT PI = M_PI;

		projectionMatrix.fill(0.0);
		projectionMatrix(0, 0) = 1.0 / tan(fov*PI / 180.0);
		projectionMatrix(1, 1) = 1.0f / tan(fov*PI / 180.0f);
		projectionMatrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
		projectionMatrix(3, 2) = -2.0* nearPlane*farPlane / (farPlane - nearPlane);
		projectionMatrix(2, 3) = -1.0;
	}

	void Camera::initializeViewMatrix()
	{
		lookAt(VECTOR3(0, 0, 0), VECTOR3(0, 0, -1), VECTOR3(0, 1, 0));
	}

	void Camera::lookAt(const VECTOR3 & eye, const VECTOR3 & target, const VECTOR3 & upVector)
	{
		VECTOR3 forward = (target - eye).normalized();
		VECTOR3 up = upVector.normalized();
		VECTOR3 right = forward.cross(up).normalized();
		up = right.cross(forward);

		viewMatrix << 
			right.x(),		right.y(),		right.z(),		-right.dot(eye),
			up.x(),				up.y(),				up.z(),				-up.dot(eye),
			-forward.x(), -forward.y(), -forward.z(), forward.dot(eye),
			0,						0,						0,						1;
	}
}