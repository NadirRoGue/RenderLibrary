#ifndef __CPU_COMPONENTLIST__
#define __CPU_COMPONENTLIST__

#include "Component.h"

#include <vector>
#include <list>

namespace RenderLib
{
	class ComponentList
	{
	private:
		std::vector<Component*> componentList;
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
			T * comp = new T();
			Component * component = dynamic_cast<Component*>(comp);

			if (component != NULL)
			{
				component->initialize();
				componentList.push_back(component);

				return comp;
			}

			return NULL;
		}

		template<class T>
		void removeComponent()
		{
			for (auto & c : componentList)
			{
				if (dynamic_cast<T*>(c) != NULL)
				{
					destroyComponent(c);
					break;
				}
			}
		}

		template<class T>
		void removeAllComponents()
		{
			for (auto & c : componentList)
			{
				if (dynamic_cast<T*>(c) != NULL)
				{
					destroyComponent(c);
				}
			}
		}

		template<class T>
		T * getComponent()
		{
			T * ptr = NULL;
			for (auto & c : componentList)
			{
				if ((ptr = dynamic_cast<T*>(c)) != NULL)
				{
					return ptr;
				}
			}

			return NULL;
		}
		/*
		template<class T>
		T * getComponent()
		{
			for (auto & c : componentList)
			{
				T * ptr = dynamic_cast<T*>(c);
				if (ptr != NULL)
				{
					return ptr;
				}
			}

			return NULL;
		}
		*/
		template<class T>
		std::vector<T*> getComponentsOfType()
		{
			std::list<T*> bufer;

			for (auto & c : componentList)
			{
				T * ptr = dynamic_cast<T*>(c);
				if (ptr != NULL)
				{
					bufer.push_back(ptr);
				}
			}

			std::vector<T*> result = { std::make_move_iterator(std::begin(bufer)), std::make_move_iterator(std::end(bufer)) };

			return result;
		}

		const std::vector<Component*> getAllComponents()
		{
			return componentList;
		}

	private:
		void destroyComponent(Component * component)
		{
			component->destroy();
			delete component;
		}
	};
}

#endif