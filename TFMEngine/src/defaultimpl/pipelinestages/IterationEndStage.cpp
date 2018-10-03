#include "defaultimpl/pipelinestages/IterationEndStage.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void IterationEndStage::runStage()
		{
			// Swap upload/render buffer for dynamic meshes
			engineInstance->getGPUMeshManager().swapDynamicBuffers();

			// Update time
			engineInstance->acquireContext();
			double elapsedTime = engineInstance->getWindow()->elapsedTimeSinceStart();
			engineInstance->getTime().update(elapsedTime);
			engineInstance->releaseContext();
		}

		void IterationEndStage::postRunStage()
		{
			engineInstance->acquireContext();

			// Clear programs created on this context
			engineInstance->getProgramManager().clear();

			engineInstance->releaseContext();

			// Non context-related clean ups
			engineInstance->getPipelineManager().getThreadPool().shutDown();
		}
	}
}