#include "SceneObject.h"

#include "EngineInstance.h"
#include "SceneManager.h"

namespace RenderLib
{
	SceneObjectPtr SceneObject::createObject(const std::string & name, SceneObject * parent)
	{
		return std::make_unique<SceneObject>(name, parent);
	}

	SceneObject::SceneObject()
		: SceneObject("", NULL)
	{

	}

	SceneObject::SceneObject(const std::string & name)
		:SceneObject(name, NULL)
	{

	}

	SceneObject::SceneObject(const std::string & name, SceneObject * parent)
		: objectName(name)
	{
		transform.object = this;

		// Avoid circular dependency
		if (parent != NULL && parent->parent != this)
		{
			this->parent = parent;
		}
	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::setParent(SceneObject * object)
	{
		if (this->parent != NULL)
		{
			this->parent->removeChildren(this);
		}

		// Avoid circular dependency
		if (object != NULL && object->parent != this)
		{
			this->parent = object;
			object->children.push_back(this);
		}
	}

	void SceneObject::addChildren(SceneObject * object)
	{
		if (object != NULL)
		{
			if (object->parent != NULL && object->parent != this)
			{
				object->parent->removeChildren(object);
			}
			
			object->parent = this;
			object->children.push_back(object);
		}
	}

	void SceneObject::removeChildren(SceneObject * object)
	{
		if (object->parent == this)
		{
			auto it = children.begin();
			while (it != children.end())
			{
				if ((*it) == object)
				{
					children.erase(it);
					break;
				}
			}
		}
	}
}