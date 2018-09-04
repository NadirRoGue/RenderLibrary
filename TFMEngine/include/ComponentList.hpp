#ifndef __RENDERLIB_COMPONENTLIST__
#define __RENDERLIB_COMPONENTLIST__

#include "Component.h"

#include <list>
#include <memory>

#include "EngineInstance.h"

namespace RenderLib
{
	class ComponentList
	{
	private:
		std::list<std::unique_ptr<Component>> componentList;

		EngineInstance * engine;
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

		void setEngineInstance(EngineInstance * instance)
		{
			engine = instance;
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
				componentList.push_back(std::move(comp));

				if (engine)
				{
					engine->getPipelineManager().registerComponent(component);
				}

				return result;
			}
			
			comp.reset();

			return NULL;
		}

		template<class T>
		void removeComponent()
		{
			std::list<std::unique_ptr<Component>>::iterator it = componentList.begin();
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
			std::list<std::unique_ptr<Component>>::iterator it = componentList.begin();
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
		std::list<T*> getComponentsOfType()
		{
			std::list<T*> bufer;

			for (auto & c : componentList)
			{
				T * ptr = dynamic_cast<T*>(c.get());
				if (ptr != NULL)
				{
					bufer.push_back(ptr);
				}
			}

			//std::vector<T*> result = { std::make_move_iterator(std::begin(bufer)), std::make_move_iterator(std::end(bufer)) };

			return bufer;
		}

		const std::list<std::unique_ptr<Component>> & getAllComponents()
		{
			return componentList;
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