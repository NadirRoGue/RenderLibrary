#ifndef __RENDERLIB_COMPONENTLIST__
#define __RENDERLIB_COMPONENTLIST__

#include "Component.h"

#include <vector>
#include <memory>

namespace RenderLib
{
	class ComponentList
	{
	private:
		std::vector<std::unique_ptr<Component>> unregistedComponents;
		std::vector<std::unique_ptr<Component>> componentList;
	public:
		ComponentList()
		{
		}

		~ComponentList()
		{
			for (auto & c : componentList)
			{
				destroyComponent(c);
			}
		}

		template<class T>
		T * addComponent()
		{
			std::unique_ptr<T> comp = std::make_unique<T>();
			Component * component = dynamic_cast<Component*>(comp.get());

			if (component != NULL)
			{
				component->initialize();
				T * result = comp.get();
				unregistedComponents.push_back(std::move(comp));

				return result;
			}
			
			comp.reset();
			return NULL;
		}

		template<class T>
		void removeComponent()
		{
			auto it = componentList.begin();
			while (it != componentList.end())
			{
				if (dynamic_cast<T*>((*it).get()) != NULL)
				{
					destroyComponent(*it);
					componentList.erase(it);
					break;
				}
				it++;
			}
		}

		template<class T>
		void removeAllComponents()
		{
			auto it = componentList.begin();
			while (it != componentList.end())
			{
				if (dynamic_cast<T*>((*it).get()) != NULL)
				{
					destroyComponent(*it);
					componentList.erase(it);
				}
				it++;
			}
		}

		template<class T>
		T * getComponent()
		{
			T * ptr = NULL;
			for (auto & c : componentList)
			{
				if ((ptr = dynamic_cast<T*>(c.get())) != NULL)
				{
					return ptr;
				}
			}

			return NULL;
		}
		
		template<class T>
		std::vector<T*> getComponentsOfType()
		{
			std::vector<T*> bufer;

			for (auto & c : componentList)
			{
				T * ptr = dynamic_cast<T*>(c.get());
				if (ptr != NULL)
				{
					bufer.push_back(ptr);
				}
			}

			return bufer;
		}

		const std::vector<std::unique_ptr<Component>> & getAllComponents()
		{
			return componentList;
		}

		std::vector<std::unique_ptr<Component>> & getUnregisteredComponents()
		{
			return unregistedComponents;
		}

		void registerComponent(std::unique_ptr<Component> & comp)
		{
			componentList.push_back(std::move(comp));
		}

	private:
		void destroyComponent(std::unique_ptr<Component> & component)
		{
			component.get()->destroy();
			component.reset();
		}
	};
}

#endif