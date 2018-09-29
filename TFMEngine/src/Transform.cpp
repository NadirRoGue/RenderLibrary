#include "Transform.h"

#include <Eigen/Geometry>

#include <iostream>

namespace RenderLib
{
	Transform::Transform()
	{
		translationV = VECTOR3(0, 0, 0);
		scaleV = VECTOR3(1, 1, 1);
		rotationV = QUATERNION::Identity();
		needsUpdate = false;
	}

	Transform::~Transform()
	{
	}

	void Transform::translate(VECTOR3 translate)
	{
		this->translationV += translate;
	}

	void Transform::setTranslation(VECTOR3 translation)
	{
		this->translationV = translation;
	}

	void Transform::scale(VECTOR3 scale)
	{
		this->scaleV.cwiseProduct(scale);
	}

	void Transform::setScale(VECTOR3 scalation)
	{
		this->scaleV = scalation;
	}

	void Transform::rotate(VECTOR3 axis, FLOAT angle)
	{
		QUATERNION add; 
		add = Eigen::AngleAxis<FLOAT>(angle, axis.normalized());

		this->rotationV *= add;
	}

	void Transform::setRotation(QUATERNION rotation)
	{
		this->rotationV = rotation;
	}

	void Transform::update()
	{
		TRANSFORM transform = TRANSFORM::Identity();
		transform.scale(scaleV);
		transform.rotate<QUATERNION>(rotationV);
		transform.translate(translationV);

		modelMatrix = transform.matrix();

		forwardVector = VECTOR3(modelMatrix(0, 2), modelMatrix(1, 2), modelMatrix(2, 2));
		upVector = VECTOR3(modelMatrix(0, 1), modelMatrix(1, 1), modelMatrix(2, 1));
		rightVector = VECTOR3(modelMatrix(0, 0), modelMatrix(1, 0), modelMatrix(2, 0));
	}
}