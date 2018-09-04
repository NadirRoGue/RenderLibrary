#ifndef __RENDER_LIB_PIPELINEBROKER_H__
#define __RENDER_LIB_PIPELINEBROKER_H__

#include <map>
#include <vector>

#include <typeinfo>
#include <typeindex>

#include "Component.h"

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	class PipelineBroker
	{
	private:
		std::map<std::type_index, std::vector<AbstractElementBasedStage*>> elementStages;
	public:
		PipelineBroker();
		~PipelineBroker();

		//template<class T>
		void registerPipelineStage(PipelineStage * stage);
		/*{
			T * newStage = new T();
			AbstractElementBasedStage * aebs = dynamic_cast<AbstractElementBasedStage*>(newStage);

			if (aebs != NULL)
			{
				elementStages[aebs->getAssociatedElementType()].push_back(aebs);
			}
			else
			{
				delete newStage;
			}

			return newStage;
		}*/

		void registerElement(Component * component);

		const std::map<std::type_index, std::vector<AbstractElementBasedStage*>> & getElementStages();
	};
}

#endif