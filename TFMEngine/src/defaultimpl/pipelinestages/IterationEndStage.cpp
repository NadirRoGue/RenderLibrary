#include "defaultimpl/pipelinestages/IterationEndStage.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void IterationEndStage::runStage()
		{
			engineInstance->getGPUMeshManager().swapDynamicBuffers();
		}
	}
}