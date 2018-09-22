#include "render/RenderStage.h"

#include "components/MeshRenderer.h"

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
			// Sort all elements
			Pipeline::ElementBasedStage<Components::MeshRenderer>::preRunStage();
			// Add them sorted to the correspondent places
			renderPipeline.initializeStages();
		}

		void RenderStage::runStage()
		{
			renderPipeline.executePipelineIteration();
		}

		void RenderStage::postRunStage()
		{
			renderPipeline.finalizeStages();
		}
	}
}