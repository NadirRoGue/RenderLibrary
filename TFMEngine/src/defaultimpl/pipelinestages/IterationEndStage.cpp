#include "defaultimpl/pipelinestages/IterationEndStage.h"

#include <iostream>

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

			std::cout << glGetError() << std::endl;

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

			// Call component's destroy method
			for (auto & obj : engineInstance->getSceneManager().getActiveScene()->getSceneObjects())
			{
				SceneObject * objPtr = obj.get();
				for (auto & comp : objPtr->getAllComponents())
				{
					comp.get()->destroy();
				}
			}
		}
	}
}