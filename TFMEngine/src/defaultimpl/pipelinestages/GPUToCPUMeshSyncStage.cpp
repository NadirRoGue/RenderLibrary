#include "defaultimpl/pipelinestages/GPUToCPUMeshSyncStage.h"

#include "CPU/memory/MemoryManager.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void GPUToCPUMeshSyncStage::registerElement(Component * comp)
		{
			DefaultImpl::MeshRenderer * mr =
				static_cast<DefaultImpl::MeshRenderer*>(comp);

			switch (mr->gpuToCpuSync)
			{
			case GPU_SYNC_CONTINOUSLY:
				syncContinousMeshes.push_back(mr->gpuMesh);
				break;
			case GPU_SYNC_ONCE_AT_END:
				syncOnceMeshes.push_back(mr->gpuMesh);
				break;
			}
		}

		void GPUToCPUMeshSyncStage::runStage()
		{
			updateCPUMeshes(syncContinousMeshes);
		}

		void GPUToCPUMeshSyncStage::postRunStage()
		{
			updateCPUMeshes(syncOnceMeshes);
		}

		void GPUToCPUMeshSyncStage::updateCPUMeshes(
			std::vector<GPU::Mesh::GPUMesh *> & list)
		{
			GPU::Mesh::GPUBuffer * dynamicBuffer = 
				engineInstance->getGPUMeshManager().getSyncDynamicBuffer();
			dynamicBuffer->bindDataBuffer();

			char * data = reinterpret_cast<char*>(
				glMapBufferRange(
					GL_ARRAY_BUFFER, 
					0, 
					dynamicBuffer->getDataSizeBytes(), 
					GL_MAP_READ_BIT));

			CPU::Memory::MemoryPool * memPool = 
				CPU::Memory::MemoryManager::getInstance().getMemoryPool<CPU::Mesh::Mesh>();

			if (memPool == NULL)
			{
				throw EngineException(
					"GPUToCPUSyncStage: No memory pool for CPU::Mesh::Mesh found. Aborting sync");
			}

			auto & blockMap = memPool->getBlocksByIndex();

			for (auto gpuMesh : list)
			{
				size_t index = gpuMesh->index;

				auto blockIt = blockMap.find(index);
				if (blockIt != blockMap.end())
				{
					CPU::Memory::MemoryBlock * block = blockIt->second;
					memPool->setBlockData(
						block,
						gpuMesh->faceIndexOffset,
						gpuMesh->dataSize,
						data + gpuMesh->dataIndexOffset);
				}
			}
		}
	}
}