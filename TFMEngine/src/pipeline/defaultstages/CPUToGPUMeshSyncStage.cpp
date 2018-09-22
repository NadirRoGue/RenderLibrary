#include "pipeline/defaultstages/CPUToGPUMeshSyncStage.h"

#include "GPU/mesh/GPUMeshManager.h"

namespace RenderLib
{
	namespace Pipeline
	{
		void CPUToGPUMeshSyncStage::registerElement(Component * component)
		{
			Components::MeshRenderer * renderable = static_cast<Components::MeshRenderer*>(component);
			switch (renderable->cpuToGpuSync)
			{
			case Components::CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING:
				syncOnce.push_back(renderable);
				break;
			case Components::CPUToGPUSyncPolicy::CPU_SYNC_CONTINOUSLY:
				syncContinously.push_back(renderable);
				break;
			}
		}

		void CPUToGPUMeshSyncStage::preRunStage()
		{
			size_t syncOnceOffset = 0;
			for (auto mesh : syncOnce)
			{
				CPU::Mesh::Mesh * cpuMesh = mesh->getCPUMesh();
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				GPU::Mesh::GPUMesh * gpuMesh = buildGPUMeshFromCPUMesh(cpuMesh, syncOnceOffset, true);

				mesh->gpuMesh = gpuMesh;

				syncOnceOffset += block->length;
			}
			// Sync-once and Sync-continously mesh synchronization
		}

		void CPUToGPUMeshSyncStage::runStage()
		{
			// Sync-continously mesh synchroniztion
		}

		GPU::Mesh::GPUMesh * CPUToGPUMeshSyncStage::buildGPUMeshFromCPUMesh(CPU::Mesh::Mesh * cpuMesh, size_t offset, bool staticMesh)
		{
			GPU::Mesh::GPUMesh * gpuMesh = GPU::Mesh::GPUMeshManager::getInstance().getGPUMesh(cpuMesh->index, staticMesh);
			if (gpuMesh == NULL)
			{
				gpuMesh = GPU::Mesh::GPUMeshManager::getInstance().createGPUMesh(cpuMesh->index, staticMesh);

				gpuMesh->faces = GPU::Mesh::GPUAttribute(sizeof(unsigned int) * 3, cpuMesh->faces.size(), cpuMesh->faces.getOffset(), cpuMesh->faces.getStride());
				gpuMesh->vertices = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 3, cpuMesh->vertices.size(), cpuMesh->vertices.getOffset(), cpuMesh->vertices.getStride());
				gpuMesh->normals = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 3, cpuMesh->normals.size(), cpuMesh->normals.getOffset(), cpuMesh->normals.getStride());
				gpuMesh->tangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 3, cpuMesh->tangents.size(), cpuMesh->tangents.getOffset(), cpuMesh->tangents.getStride());
				gpuMesh->bitangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 3, cpuMesh->bitangents.size(), cpuMesh->bitangents.getOffset(), cpuMesh->bitangents.getStride());

				gpuMesh->uvs.resize(cpuMesh->uvs.size());
				size_t i = 0;
				for (auto & uvAttrib : cpuMesh->uvs)
				{
					gpuMesh->uvs[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 2, uvAttrib.size(), uvAttrib.getOffset(), uvAttrib.getStride());
					i++;
				}

				gpuMesh->colors.resize(cpuMesh->colors.size());
				i = 0;
				for (auto & colorAttrib : cpuMesh->colors)
				{
					gpuMesh->colors[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT) * 4, colorAttrib.size(), colorAttrib.getOffset(), colorAttrib.getStride());
					i++;
				}

				gpuMesh->faceIndexOffset = offset;
			}

			return gpuMesh;
		}
	}
}