#include "pipeline/defaultstages/CPUToGPUMeshSyncStage.h"

namespace RenderLib
{
	namespace Pipeline
	{
		void CPUToGPUMeshSyncStage::registerElement(Component * component)
		{
			Components::MeshRenderer * renderable = static_cast<Components::MeshRenderer*>(component);
		}

		void CPUToGPUMeshSyncStage::preRunStage()
		{
			// Sync-once mesh synchronization
		}

		void CPUToGPUMeshSyncStage::runStage()
		{
			// Sync-continously mesh synchroniztion
		}
	}
}