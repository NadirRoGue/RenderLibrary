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
		projectionMatrix.fill(0.0);
		projectionMatrix(0, 0) = FLOAT(1.0) / tan(fov*M_PI / FLOAT(180.0));
		projectionMatrix(1, 1) = FLOAT(1.0) / tan(fov*M_PI / FLOAT(180.0));
		projectionMatrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
		projectionMatrix(3, 2) = FLOAT(-2.0) * nearPlane * farPlane / (farPlane - nearPlane);
		projectionMatrix(2, 3) = FLOAT(-1.0);
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

	void Camera::translateView(const VECTOR3 & translation)
	{

	}

	void Camera::rotateView(const VECTOR3 & angles)
	{

	}

	void Camera::updateViewMatrix()
	{

	}

	void Camera::onWindowResize(const int & width, const int & height)
	{
		float fWidth = (float)width;
		float fHeight = (float)height;

		projectionMatrix.fill(0.0);
		projectionMatrix(0, 0) = FLOAT(1.0) / (tan(fov*M_PI / FLOAT(180.0)) * (fWidth / fHeight));
		projectionMatrix(1, 1) = FLOAT(1.0) / tan(fov*M_PI / FLOAT(180.0));
		projectionMatrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
		projectionMatrix(3, 2) = FLOAT(-2.0) * nearPlane * farPlane / (farPlane - nearPlane);
		projectionMatrix(2, 3) = FLOAT(-1.0);
	}
}