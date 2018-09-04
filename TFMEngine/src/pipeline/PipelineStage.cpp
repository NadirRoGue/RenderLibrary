#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	PipelineStage::PipelineStage()
	{

	}

	PipelineStage::~PipelineStage()
	{
	
	}

	AbstractElementBasedStage::AbstractElementBasedStage()
	{

	}

	AbstractElementBasedStage::~AbstractElementBasedStage()
	{

	}

	void AbstractElementBasedStage::registerElement(Component * comp)
	{
		if (comp != NULL)
		{
			elements.push_back(comp);
		}
	}
}