#include "defaultimpl/components/ObjectSpinner.h"

#include "SceneObject.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void ObjectSpinner::update()
		{
			double delta = engineInstance->getTime().deltaTime();

			object->transform.rotate(VECTOR3(0, 1, 0), (FLOAT)1.0 * delta);
		}
	}
}