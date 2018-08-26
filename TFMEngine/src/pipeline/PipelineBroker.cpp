#include "pipeline/PipelineBroker.h"

namespace RenderLib
{
	PipelineBroker::PipelineBroker()
	{

	}

	PipelineBroker::~PipelineBroker()
	{

	}

	void PipelineBroker::registerElement(Component * component)
	{
		std::type_index type = component->getComponentType();

		std::map<std::type_index, std::vector<AbstractElementBasedStage*>>::iterator it = elementStages.find(type);
		if (it != elementStages.end())
		{
			for (auto stage : it->second)
			{
				stage->registerElement(component);
			}
		}
	}

	const std::map<std::type_index, std::vector<AbstractElementBasedStage*>> & PipelineBroker::getElementStages()
	{
		return elementStages;
	}
}