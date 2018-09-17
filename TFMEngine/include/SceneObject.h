#ifndef __CPU_SCENEOBJECT__
#define __CPU_SCENEOBJECT__

#include <vector>
#include <string>
#include <memory>

#include "ComponentList.hpp"
#include "Transform.h"

namespace RenderLib
{
	typedef std::unique_ptr<SceneObject> SceneObjectPtr;

	class SceneObject
	{
	public:
		static SceneObjectPtr createObject(const std::string & name = "", SceneObject * parent = NULL);
	public:
		// Transformation state for this object
		Transform transform;
		// Wether this object is enabled for processing
		bool active;
		// Object name
		std::string objectName;
	private:
		// List with all the components attached to this object
		ComponentList componentList;

		// Parent object
		SceneObject * parent;
		
		// Child objects
		std::vector<SceneObject *> children;
	
	public:
		SceneObject();
		SceneObject(const std::string & name);
		SceneObject(const std::string & name, SceneObject * parent = NULL);
		~SceneObject();

		void setParent(SceneObject * object);
		void addChildren(SceneObject * object);
		void removeChildren(SceneObject * object);

		ComponentList & getComponentList();

		template<class T>
		T * addComponent()
		{
			T * comp = componentList.addComponent<T>();
			if(comp)
			{
				Component * compInstance = static_cast<Component*>(comp);
				compInstance->object = this;
			}

			return comp;
		}

		template<class T>
		void removeComponent()
		{
			componentList.removeComponent<T>();
		}

		template<class T>
		T * getComponent()
		{
			return componentList.getComponent<T>();
		}

		const std::vector<std::unique_ptr<Component>> & getAllComponents()
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