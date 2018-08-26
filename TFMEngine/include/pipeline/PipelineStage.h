#ifndef __CPU_PIPELINE_STAGE_H__
#define __CPU_PIPELINE_STAGE_H__

#include <vector>

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

		void registerElement(T * element)
		{
			elements.push_back(element);
		}

		void runStage()
		{
			for (auto e : elements)
			{
				processElement(static_cast<T*>(e));
			}
		}

		virtual void processElement(T * element) = 0;
	};
}

#endif