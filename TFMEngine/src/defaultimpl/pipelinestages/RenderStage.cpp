#include "defaultimpl/pipelinestages/RenderStage.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/ProgramManager.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void RenderStage::registerElement(Component * comp)
		{
			MeshRenderer * meshRenderer = static_cast<MeshRenderer*>(comp);

			if (meshRenderer->cpuToGpuSync != CPUToGPUSyncPolicy::CPU_DO_NOT_SYNC)
			{
				renderPipeline.registerRenderable(meshRenderer);
			}
		}

		void RenderStage::preRunStage()
		{
			engineInstance->acquireContext();

			// Sort all elements
			Pipeline::ElementBasedStage<MeshRenderer>::preRunStage();
			// Add them sorted to the correspondent places
			
			renderPipeline.initializeStages(engineInstance);

			engineInstance->releaseContext();
		}

		void RenderStage::runStage()
		{
			engineInstance->acquireContext();

			renderPipeline.executePipelineIteration();

			engineInstance->releaseContext();
		}

		void RenderStage::postRunStage()
		{
			engineInstance->acquireContext();

			renderPipeline.finalizeStages();

			engineInstance->releaseContext();

			//GPU::Program::ProgramManager::getInstance().clear();
		}
	}
}