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
	namespace Pipeline
	{
		class PipelineBroker
		{
		private:
			std::map<std::type_index, std::vector<AbstractElementBasedStage*>> elementStages;
		public:
			PipelineBroker();
			~PipelineBroker();

			void registerPipelineStage(PipelineStage * stage);

			void registerElement(Component * component);

			const std::map<std::type_index, std::vector<AbstractElementBasedStage*>> & getElementStages();
		};
	}
}

#endif