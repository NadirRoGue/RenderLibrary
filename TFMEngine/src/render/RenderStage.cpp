#include "render/RenderStage.h"

#include "components/MeshRenderer.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		void RenderStage::registerElement(Component * comp)
		{
			Components::MeshRenderer * meshRenderer = static_cast<Components::MeshRenderer*>(comp);

			if (meshRenderer->cpuToGpuSync != Components::CPUToGPUSyncPolicy::CPU_DO_NOT_SYNC)
			{
				renderPipeline.registerRenderable(meshRenderer);
			}
		}

		void RenderStage::preRunStage()
		{
			engineInstance->acquireContext();

			// Sort all elements
			Pipeline::ElementBasedStage<Components::MeshRenderer>::preRunStage();
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
		}
	}
}