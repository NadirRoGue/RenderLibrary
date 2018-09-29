#include "pipeline/PipelineManager.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Pipeline
	{
		PipelineManager::PipelineManager(EngineInstance * engInstance)
		{
			engineInstance = engInstance;
		}

		PipelineManager::~PipelineManager()
		{

		}

		Pipeline & PipelineManager::getPipeline()
		{
			return pipeline;
		}

		PipelineBroker & PipelineManager::getStageBroker()
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