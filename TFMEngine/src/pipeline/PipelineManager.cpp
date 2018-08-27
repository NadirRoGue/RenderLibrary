#include "pipeline/PipelineManager.h"

namespace RenderLib
{
	PipelineManager::PipelineManager()
	{

	}

	PipelineManager::~PipelineManager()
	{

	}

	const Pipeline & PipelineManager::getPipeline()
	{
		return pipeline;
	}

	const PipelineBroker & PipelineManager::getStageBroker()
	{
		return stageBroker;
	}

	void PipelineManager::registerComponent(Component * component)
	{
		stageBroker.registerElement(component);
	}

	void PipelineManager::executePipeline()
	{
		pipeline.executePipeline();
	}
}