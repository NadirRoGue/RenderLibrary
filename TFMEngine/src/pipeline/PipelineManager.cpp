#include "pipeline/PipelineManager.h"

namespace RenderLib
{
	namespace Pipeline
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

		void PipelineManager::removeComponent(Component * component)
		{

		}

		void PipelineManager::initializeStages()
		{
			for (auto & stagePtr : pipeline.getAllStages())
			{
				PipelineStage * stage = stagePtr.get();
				if (stage)
				{
					stage->preRunStage();
				}
			}
		}

		void PipelineManager::finishStages()
		{
			for (auto & stagePtr : pipeline.getAllStages())
			{
				PipelineStage * stage = stagePtr.get();
				if (stage)
				{
					stage->postRunStage();
				}
			}
		}

		void PipelineManager::executePipeline()
		{
			pipeline.execute();
		}
	}
}