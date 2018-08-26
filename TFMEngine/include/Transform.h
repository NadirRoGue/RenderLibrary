#ifndef __CPU_TRANSFORM__
#define __CPU_TRANSFORM__

#include "Defines.h"

namespace RenderLib
{
	class SceneObject;

	class Transform
	{
	public:
		MATRIX4 modelMatrix;

		VECTOR3 translationV;
		VECTOR3 scaleV;
		QUATERNION rotationV;

		VECTOR3 forwardVector;
		VECTOR3 rightVector;
		VECTOR3 upVector;

		SceneObject * object;

	public:
		Transform();
		~Transform();

		void translate(VECTOR3 translate);
		void setTranslation(VECTOR3 translation);
		
		void scale(VECTOR3 scalation);
		void setScale(VECTOR3 scale);

		void rotate(VECTOR3 axis, FLOAT angleRadians);
		void setRotation(QUATERNION rotation);
	};
}

#endif