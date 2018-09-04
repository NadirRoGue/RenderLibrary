#include "SceneObject.h"

namespace RenderLib
{
	SceneObject::SceneObject(SceneObject * parent)
		:parent(parent)
	{
		transform.object = this;
	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::setEngine(EngineInstance * instance)
	{
		componentList.setEngineInstance(instance);
	}

	void SceneObject::setParent(SceneObject * object)
	{
		parent = object;
	}
}