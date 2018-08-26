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
		elements.push_back(comp);
	}
}