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
			// Sort all elements
			Pipeline::ElementBasedStage<Components::MeshRenderer>::preRunStage();
			// Add them sorted to the correspondent places
			
			GPU::Mesh::GPUMeshManager & gpuMeshManager = engineInstance->getGPUMeshManager();

			renderPipeline.initializeStages(&gpuMeshManager);
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