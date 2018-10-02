#include "SceneObject.h"

#include "EngineInstance.h"
#include "SceneManager.h"

namespace RenderLib
{
	SceneObject::SceneObject()
		: initialized(false)
		, parent(NULL)
	{
		transform.object = this;
		transform.update();
	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::initialize()
	{
		initialized = true;
	}

	ComponentList & SceneObject::getComponentList()
	{
		return componentList;
	}

	void SceneObject::setParent(SceneObject * parentToBe)
	{
		if (parentToBe == this)
		{
			return;
		}
		
		if (this->parent != NULL)
		{
			this->parent->removeChildren(this);
		}

		// Avoid circular dependency
		if (parentToBe != NULL && parentToBe->parent != this)
		{
			this->parent = parentToBe;
			parentToBe->children.push_back(this);
		}
	}

	void SceneObject::addChildren(SceneObject * object)
	{
		if (object == this)
		{
			return;
		}

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
	
	SceneObject * SceneObject::getParent()
	{
		return parent;
	}

	std::vector<SceneObject*> & SceneObject::getChildren()
	{
		return children;
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