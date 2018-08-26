#ifndef __CPU_SCENEOBJECT__
#define __CPU_SCENEOBJECT__

#include "ComponentList.hpp"
#include "Transform.h"

namespace RenderLib
{
	class Component;

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
	public:
		SceneObject();
		~SceneObject();

		template<class T>
		void addComponent()
		{
			componentList.addComponent<T>();
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

		std::vector<Component*> getAllComponents()
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