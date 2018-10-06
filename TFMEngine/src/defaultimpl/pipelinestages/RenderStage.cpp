#include "defaultimpl/pipelinestages/RenderStage.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/ProgramManager.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void RenderStage::preRunStage()
		{
			// Sort all elements
			Pipeline::ElementBasedStage<MeshRenderer>::preRunStage();

			engineInstance->acquireContext();

			renderPipeline.setEngineInstance(engineInstance);

			// Add them sorted to the correspondent places
			for (auto comp : elements)
			{
				renderPipeline.registerRenderable(static_cast<MeshRenderer*>(comp));
			}

			renderPipeline.initializeStages();

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