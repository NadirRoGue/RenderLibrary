#include "pipeline/PipelineManager.h"

#include "pipeline/defaultstages/ComponentRegisterStage.h"
#include "pipeline/defaultstages/CPUToGPUMeshSyncStage.h"

namespace RenderLib
{
	namespace Pipeline
	{
		PipelineManager::PipelineManager()
		{
			addPipelineStage<ComponentRegisterStage>();
			addPipelineStage<CPUToGPUMeshSyncStage>();
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

		void PipelineManager::initializeStages(Scene * scene)
		{
			for (auto & stagePtr : pipeline.getAllStages())
			{
				PipelineStage * stage = stagePtr.get();
				if (stage)
				{
					stage->scene = scene;
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