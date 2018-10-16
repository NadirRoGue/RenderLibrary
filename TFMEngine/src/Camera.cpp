#include "Camera.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)

namespace RenderLib
{
	Camera::Camera()
		: initWidth(-1)
		, initHeight(-1)
	{
		totalRotation.setZero();
	}

	Camera::~Camera()
	{

	}

	void Camera::setWindowSize(int width, int height)
	{
		if (initWidth < 0)
		{
			initWidth = static_cast<FLOAT>(width);
		}

		if (initHeight < 0)
		{
			initHeight = static_cast<FLOAT>(height);
		}
		onWindowResize(width, height);
	}

	void Camera::initialize()
	{
		SceneObject::initialize();
		initializeProjectionMatrix();
	}

	void Camera::setProjectionParams(const FLOAT & nearPlane, const FLOAT & farPlane, const FLOAT & FOV)
	{
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->fov = FOV;
	}

	void Camera::initializeProjectionMatrix()
	{
		projectionMatrix.fill(0.0);
		FLOAT rads = (FLOAT)degreesToRadians(fov);
		projectionMatrix(0, 0) = FLOAT(1.0) / tan(rads);
		projectionMatrix(1, 1) = FLOAT(1.0) / tan(rads);
		projectionMatrix(2, 2) = (farPlane + nearPlane) / (nearPlane - farPlane);
		projectionMatrix(3, 2) = FLOAT(2.0) * nearPlane * farPlane / (nearPlane - farPlane);
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

		// Update rotation
		transform.rotationV.setFromTwoVectors(transform.forwardVector, forward);
		// Update translation
		transform.translationV = eye;
		// Update main vectors
		transform.forwardVector = forward;
		transform.rightVector = right;
		transform.upVector = up;

		viewMatrix << 
			right.x(),		right.y(),		right.z(),		-right.dot(eye),
			up.x(),				up.y(),				up.z(),				-up.dot(eye),
			-forward.x(), -forward.y(), -forward.z(), forward.dot(eye),
			0,						0,						0,						1;

		// Update matrix
		transform.modelMatrix = viewMatrix;
	}

	void Camera::translateView(const VECTOR3 & translation)
	{
		transform.translate(translation);
		updateViewMatrix();
	}

	void Camera::rotateView(const VECTOR3 & angles)
	{
		totalRotation += angles;
		
		ANGLEAXIS roll = ANGLEAXIS(totalRotation.z(), VECTOR3::UnitZ());
		ANGLEAXIS yaw = ANGLEAXIS(totalRotation.y(), VECTOR3::UnitY());
		ANGLEAXIS pitch = ANGLEAXIS(totalRotation.x(), VECTOR3::UnitX());

		MATRIX3 yawMat = yaw.matrix();
		MATRIX3 pitchMat = pitch.matrix();

		transform.rotationV = QUATERNION(roll * pitchMat * yawMat);
		
		updateViewMatrix();
	}

	void Camera::updateViewMatrix()
	{
		transform.update();
		viewMatrix = transform.modelMatrix;
	}

	void Camera::onWindowResize(const int & width, const int & height)
	{
		FLOAT fWidth = (FLOAT)width;
		FLOAT fHeight = (FLOAT)height;

		FLOAT ratio = fWidth / fHeight;
		
		FLOAT rads = (FLOAT)degreesToRadians(fov);

		projectionMatrix.fill(0.0);
		projectionMatrix(0, 0) = FLOAT(1.0) / (ratio * tan(rads));
		projectionMatrix(1, 1) = FLOAT(1.0) / tan(rads);
		projectionMatrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
		projectionMatrix(3, 2) = (FLOAT(-2.0) * nearPlane * farPlane) / (farPlane - nearPlane);
		projectionMatrix(2, 3) = FLOAT(-1.0);
	}
}