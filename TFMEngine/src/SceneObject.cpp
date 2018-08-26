#include "SceneObject.h"

namespace RenderLib
{
	SceneObject::SceneObject()
	{
		transform.object = this;
	}

	SceneObject::~SceneObject()
	{

	}
}