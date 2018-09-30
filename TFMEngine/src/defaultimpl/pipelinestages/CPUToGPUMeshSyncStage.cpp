#include "defaultimpl/pipelinestages/CPUToGPUMeshSyncStage.h"

#include "CPU/memory/MemoryManager.h"

#include "GPU/mesh/GPUMeshManager.h"

#include "EngineInstance.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CPUToGPUMeshSyncStage::preRunStage()
		{
			// Let superclass to sort elements based on cpu memory position
			ElementBasedStage<MeshRenderer>::preRunStage();

			SourceMeshList syncOnce;
			SourceMeshList syncContinously;
			// Register to internal pools
			registerMeshes(syncOnce, syncContinously);

			// Create the GPU meshes. Gather unique meshes for synchroniztion
			// STATIC MESHES
			createGPUMeshes(syncOnce, syncOnceData, true);
			// DYNAMIC MESHES
			createGPUMeshes(syncContinously, syncContinouslyData, false);

			// Create static buffer
			GPU::Mesh::GPUBuffer * staticBuffer = engineInstance->getGPUMeshManager().initializeStaticBuffer();
			// Synchronize CPU -> GPU data
			synchronizeData(staticBuffer, syncOnceData);
			
			// Create dynamic buffer(s)
			// Creates both, returns front buffer
			GPU::Mesh::GPUBuffer * dynamicBuffer = engineInstance->getGPUMeshManager().initializeDynamicBuffer();
			synchronizeData(dynamicBuffer, syncContinouslyData);
		}

		void CPUToGPUMeshSyncStage::runStage()
		{
			// Sync-continously mesh synchroniztion
			// Register new meshes
			if (elements.size() > 0)
			{
				SourceMeshList dummy, newDynamic;
				registerMeshes(dummy, newDynamic);
				createGPUMeshes(newDynamic, syncContinouslyData, false);
			}

			GPU::Mesh::GPUBuffer * dynamicBuffer = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
			synchronizeData(dynamicBuffer, syncContinouslyData);
		}

		// ==============================================================================================

		void CPUToGPUMeshSyncStage::registerMeshes(SourceMeshList & syncOnce ,SourceMeshList & syncContinously)
		{
			if (elements.size() > 0)
			{
				for (auto comp : elements)
				{
					MeshRenderer * renderable = static_cast<MeshRenderer*>(comp);
					switch (renderable->cpuToGpuSync)
					{
					case CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING:
						syncOnce.push_back(renderable);
						break;
					case CPUToGPUSyncPolicy::CPU_SYNC_CONTINOUSLY:
						syncContinously.push_back(renderable);
						break;
					}
				}

				// Clear already registered meshes, so we dont re-process them when new meshes come
				elements.clear();
			}
		}

		void CPUToGPUMeshSyncStage::createGPUMeshes(SourceMeshList & src, GPUMeshList & dst, bool staticMeshes)
		{
			for (auto mesh : src)
			{
				CPU::Mesh::Mesh * cpuMesh = mesh->getCPUMesh();
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				GPU::Mesh::GPUMesh * gpuMesh = engineInstance->getGPUMeshManager().getGPUMesh(cpuMesh->index, staticMeshes);
				if (gpuMesh == NULL)
				{
					gpuMesh = buildGPUMeshFromCPUMesh(cpuMesh, staticMeshes);

					// Add each gpu mesh just once to the result, so we do not upload duplicated data to the video memory
					SyncData data;
					data.cpuMesh = cpuMesh;
					data.gpuMesh = gpuMesh;
					data.facesSize = gpuMesh->faces.elementTypeSize * gpuMesh->faces.elementCount * gpuMesh->faces.numElements;
					data.dataSize = (block->length - data.facesSize);

					dst.push_back(data);
				}

				mesh->gpuMesh = gpuMesh;
			}
		}

		GPU::Mesh::GPUMesh * CPUToGPUMeshSyncStage::buildGPUMeshFromCPUMesh(CPU::Mesh::Mesh * cpuMesh, bool staticMesh)
		{
			GPU::Mesh::GPUMesh * gpuMesh = engineInstance->getGPUMeshManager().createGPUMesh(cpuMesh->index, staticMesh);

			size_t vOffset = cpuMesh->vertices.getOffset();
			gpuMesh->faces = GPU::Mesh::GPUAttribute(sizeof(unsigned int), 3, cpuMesh->faces.size(), cpuMesh->faces.getOffset(), cpuMesh->faces.getStride());
			gpuMesh->vertices = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->vertices.size(), cpuMesh->vertices.getOffset() - vOffset, cpuMesh->vertices.getStride());
			gpuMesh->normals = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->normals.size(), cpuMesh->normals.getOffset() - vOffset, cpuMesh->normals.getStride());
			gpuMesh->tangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->tangents.size(), cpuMesh->tangents.getOffset() - vOffset, cpuMesh->tangents.getStride());
			gpuMesh->bitangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->bitangents.size(), cpuMesh->bitangents.getOffset() - vOffset, cpuMesh->bitangents.getStride());

			gpuMesh->uvs.resize(cpuMesh->uvs.size());
			size_t i = 0;
			for (auto & uvAttrib : cpuMesh->uvs)
			{
				gpuMesh->uvs[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 2, uvAttrib.size(), uvAttrib.getOffset() - vOffset, uvAttrib.getStride());
				i++;
			}

			gpuMesh->colors.resize(cpuMesh->colors.size());
			i = 0;
			for (auto & colorAttrib : cpuMesh->colors)
			{
				gpuMesh->colors[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 4, colorAttrib.size(), colorAttrib.getOffset() - vOffset, colorAttrib.getStride());
				i++;
			}

			return gpuMesh;
		}

		void CPUToGPUMeshSyncStage::synchronizeData(GPU::Mesh::GPUBuffer * buffer, GPUMeshList & sourceMeshes)
		{
			if (sourceMeshes.size() == 0)
			{
				return;
			}

			// Adjust meshes
			size_t faceSize = 0, dataSize = 0, baseVertexOffset = 0;
			for (auto syncData : sourceMeshes)
			{
				// Adjust current gpu mesh params
				syncData.gpuMesh->faceIndexOffset = faceSize;
				syncData.gpuMesh->verticesBaseOffset = baseVertexOffset;

				// Increase global offsets
				faceSize += syncData.facesSize;
				dataSize += syncData.dataSize;
				baseVertexOffset += syncData.cpuMesh->vertices.size();
			}

			char * faceBuffer = new char[faceSize];
			char * dataBuffer = new char[dataSize];

			size_t facesOffset = 0;
			size_t dataOffset = 0;

			CPU::Memory::MemoryPool * meshPool = CPU::Memory::MemoryManager::getInstance().getMemoryPool<CPU::Mesh::Mesh>();
			char * poolData = meshPool->getDataAsBytes();

			// Gather all data
			for (auto & data : sourceMeshes)
			{
				CPU::Mesh::Mesh * cpuMesh = data.cpuMesh;
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				// Copy indexes
				memcpy(faceBuffer + facesOffset, poolData + block->offset, data.facesSize);
				// Copy data
				memcpy(dataBuffer + dataOffset, poolData + block->offset + data.facesSize, data.dataSize);

				facesOffset += data.facesSize;
				dataOffset += data.dataSize;
			}

			// Upload to GPU
			buffer->updateData(faceBuffer, faceSize, dataBuffer, dataSize);

			// Configure gpu mesh attributes
			for (auto & data : sourceMeshes)
			{

			}

			delete[] faceBuffer;
			delete[] dataBuffer;
		}
	}
}