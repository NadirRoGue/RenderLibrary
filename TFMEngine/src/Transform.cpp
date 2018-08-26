#include "Transform.h"

#include <Eigen/Geometry>

namespace RenderLib
{
	Transform::Transform()
	{
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
}