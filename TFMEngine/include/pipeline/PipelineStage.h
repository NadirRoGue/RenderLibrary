
#ifndef __CPU_PIPELINE_STAGE_H__
#define __CPU_PIPELINE_STAGE_H__

#include <vector>
#include <iostream>

#include <typeindex>
#include <typeinfo>

#include <array>

#include "CPU/memory/SortablePoolElement.h"

#include "Component.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Pipeline
	{
		class PipelineManager;

		class PipelineStage
		{
		public:
			EngineInstance * engineInstance;
		public:
			PipelineStage();
			~PipelineStage();
			virtual void preRunStage();
			virtual void runStage() = 0;
			virtual void postRunStage();
		};

		class AbstractElementBasedStage : public PipelineStage
		{
		protected:
			std::vector<Component*> elements;
		public:
			AbstractElementBasedStage();
			~AbstractElementBasedStage();

			virtual void registerElement(Component * comp);

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

			virtual void preRunStage()
			{
				if (elements.size() > 1)
				{
					// Check whether the type of elements we are storing
					// can be sorted to access memory more eficiently
					//Component * comp = elements[0];
					//if (dynamic_cast<CPU::Memory::SortablePoolElement*>(comp) != NULL)
					if(std::is_base_of<CPU::Memory::SortablePoolElement, T>::value)
					{
						std::sort(elements.begin(), elements.end(),
						[](Component * a, Component * b) -> bool
						{
							if (a == NULL)
								return false;

							if (b == NULL)
								return true;

							CPU::Memory::SortablePoolElement * aSortable = dynamic_cast<CPU::Memory::SortablePoolElement*>(a);
							CPU::Memory::SortablePoolElement * bSortable = dynamic_cast<CPU::Memory::SortablePoolElement*>(b);

							return aSortable->getIndex() < bSortable->getIndex();
						});
					}
				}
			}

			virtual void runStage()
			{
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
			}

			virtual void processElement(T * element)
			{

			}
		};
	}
}

#endif