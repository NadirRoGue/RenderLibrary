#include "defaultimpl/components/ObjectSpinner.h"

#include "SceneObject.h"
#include "Defines.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void ObjectSpinner::update()
		{
			object->transform.rotate(VECTOR3(0, 1, 0), 0.01f);
		}
	}
}