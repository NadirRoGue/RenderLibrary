#ifndef __CPU_SCENEOBJECT__
#define __CPU_SCENEOBJECT__

#include <list>

#include "ComponentList.hpp"
#include "Transform.h"

namespace RenderLib
{
	class Component;
	class EngineInstance;

	class SceneObject
	{
	public:
		// Transformation state for this object
		Transform transform;
		// Wether this object is enabled for processing
		bool active;
	private:
		// List with all the components attached to this object
		ComponentList componentList;

		// Parent object
		SceneObject * parent;

	public:
		SceneObject(SceneObject * parent = NULL);
		~SceneObject();

		void setParent(SceneObject * object);

		void setEngine(EngineInstance * instance);

		template<class T>
		T * addComponent()
		{
			return componentList.addComponent<T>();
		}

		template<class T>
		void removeComponent()
		{
			componentList.removeComponent<T>();
		}

		template<class T>
		T getComponent()
		{
			return componentList.getComponent<T>();
		}

		const std::list<std::unique_ptr<Component>> & getAllComponents()
		{
			return componentList.getAllComponents();
		}

		template<class T>
		std::vector<T*> getAllComponentsOfType()
		{
			return componentList.getComponentsOfType<T>();
		}
	};
}

#endif