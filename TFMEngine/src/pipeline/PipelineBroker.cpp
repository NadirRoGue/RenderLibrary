#include "pipeline/PipelineBroker.h"

namespace RenderLib
{
	namespace Pipeline
	{
		PipelineBroker::PipelineBroker()
		{

		}

		PipelineBroker::~PipelineBroker()
		{

		}

		void PipelineBroker::registerPipelineStage(PipelineStage * stage)
		{
			AbstractElementBasedStage * aebs = dynamic_cast<AbstractElementBasedStage*>(stage);

			if (aebs != NULL)
			{
				elementStages[aebs->getAssociatedElementType()].push_back(aebs);
			}
		}

		void PipelineBroker::registerElement(Component * component)
		{
			std::type_index type = component->getComponentType();

			auto it = elementStages.find(type);
			if (it != elementStages.end())
			{
				for (auto stage : it->second)
				{
					stage->registerElement(component);
				}
			}
		}

		const std::unordered_map<std::type_index, std::vector<AbstractElementBasedStage*>> & PipelineBroker::getElementStages()
		{
			return elementStages;
		}
	}
}