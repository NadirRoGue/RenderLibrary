#ifndef __CPU_PIPELINE_STAGE_H__
#define __CPU_PIPELINE_STAGE_H__

#include <vector>
#include <iostream>

#include <typeindex>
#include <typeinfo>

#include "Component.h"

namespace RenderLib
{
	class PipelineStage
	{
	public:
		PipelineStage();
		~PipelineStage();
		virtual void runStage() = 0;
	};

	class AbstractElementBasedStage : public PipelineStage
	{
	protected:
		std::vector<Component*> elements;
	public:
		AbstractElementBasedStage();
		~AbstractElementBasedStage();

		void registerElement(Component * comp);

		virtual std::type_index getAssociatedElementType() = 0;
	};

	template<class T>
	class ElementBasedStage : public AbstractElementBasedStage
	{
	public:
		ElementBasedStage()
		{
		}

		~ElementBasedStage()
		{

		}

		std::type_index getAssociatedElementType()
		{
			return typeid(T);
		}

		void runStage()
		{
			std::cout << "Element stage with " << elements.size() << " element(s)" << std::endl;
			std::vector<Component*>::iterator it = elements.begin();
			while (it != elements.end())
			{
				if ((*it) != NULL)
				{
					if ((*it)->enabled)
					{
						processElement(static_cast<T*>(*it));
					}
				}
				else // Invalid/null or scheduled to delete component. Remove it
				{
					elements.erase(it);
				}

				it++;
			}
			/*
			for (auto e : elements)
			{
				if (e != NULL)
				{
					processElement(static_cast<T*>(e));
				}
			}
			*/
		}

		virtual void processElement(T * element) = 0;
	};
}

#endif