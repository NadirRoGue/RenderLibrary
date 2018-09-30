#include "defaultimpl/pipelinestages/CPUToGPUMeshSyncStage.h"

#include "CPU/memory/MemoryManager.h"

#include "GPU/mesh/GPUMeshManager.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CPUToGPUMeshSyncStage::preRunStage()
		{
			// Let superclass to sort elements based on cpu memory position
			ElementBasedStage<MeshRenderer>::preRunStage();

			// Register to internal pools
			registerMeshes();

			// Create the GPU meshes
			// STATIC MESHES
			size_t faceSizeOnce, dataSizeOnce;
			GPUMeshList syncOnceResult;
			createGPUMeshes(syncOnce, syncOnceResult, faceSizeOnce, dataSizeOnce, true);

			// DYNAMIC MESHES
			size_t faceSizeCont, dataSizeCont;
			GPUMeshList syncContResult;
			createGPUMeshes(syncContinously, syncContResult, faceSizeCont, dataSizeCont, false);

			// Create static buffer
			GPU::Mesh::GPUBuffer * staticBuffer = engineInstance->getGPUMeshManager().initializeStaticBuffer();
			// Synchronize CPU -> GPU data
			synchronizeData(staticBuffer, syncOnceResult, faceSizeOnce, dataSizeOnce);
			
			// Create dynamic buffer(s)
			// Creates both, returns front buffer
			GPU::Mesh::GPUBuffer * dynamicBuffer = engineInstance->getGPUMeshManager().initializeDynamicBuffer();
			//synchronizeData(dynamicBuffer, syncContResult, faceSizeCont, dataSizeCont);
		}

		void CPUToGPUMeshSyncStage::runStage()
		{
			// Sync-continously mesh synchroniztion
		}

		// ==============================================================================================

		void CPUToGPUMeshSyncStage::registerMeshes()
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

		void CPUToGPUMeshSyncStage::createGPUMeshes(SourceMeshList & src, GPUMeshList & dst, size_t & faceSizeBytes, size_t & dataSizeBytes, bool staticMeshes)
		{
			faceSizeBytes = dataSizeBytes = 0;

			for (auto mesh : src)
			{
				CPU::Mesh::Mesh * cpuMesh = mesh->getCPUMesh();
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				GPU::Mesh::GPUMesh * gpuMesh = engineInstance->getGPUMeshManager().getGPUMesh(cpuMesh->index, staticMeshes);
				if (gpuMesh == NULL)
				{
					gpuMesh = buildGPUMeshFromCPUMesh(cpuMesh, true);
					gpuMesh->faceIndexOffset = faceSizeBytes;
					gpuMesh->dataIndexOffset = dataSizeBytes;
					// Add each gpu mesh just once to the result, so we do not upload duplicated data to the video memory
					SyncData data;
					data.cpuMesh = cpuMesh;
					data.gpuMesh = gpuMesh;
					data.facesSize = gpuMesh->faces.elementTypeSize * gpuMesh->faces.elementCount * gpuMesh->faces.numElements;
					data.dataSize = (block->length - data.facesSize);

					dataSizeBytes += data.dataSize;
					faceSizeBytes += data.facesSize;

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
			gpuMesh->vertices = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->vertices.size(), vOffset - cpuMesh->vertices.getOffset(), cpuMesh->vertices.getStride());
			gpuMesh->normals = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->normals.size(), vOffset - cpuMesh->normals.getOffset(), cpuMesh->normals.getStride());
			gpuMesh->tangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->tangents.size(), vOffset - cpuMesh->tangents.getOffset(), cpuMesh->tangents.getStride());
			gpuMesh->bitangents = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 3, cpuMesh->bitangents.size(), vOffset - cpuMesh->bitangents.getOffset(), cpuMesh->bitangents.getStride());

			gpuMesh->uvs.resize(cpuMesh->uvs.size());
			size_t i = 0;
			for (auto & uvAttrib : cpuMesh->uvs)
			{
				gpuMesh->uvs[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 2, uvAttrib.size(), uvAttrib.getOffset(), uvAttrib.getStride());
				i++;
			}

			gpuMesh->colors.resize(cpuMesh->colors.size());
			i = 0;
			for (auto & colorAttrib : cpuMesh->colors)
			{
				gpuMesh->colors[i] = GPU::Mesh::GPUAttribute(sizeof(FLOAT), 4, colorAttrib.size(), colorAttrib.getOffset(), colorAttrib.getStride());
				i++;
			}

			return gpuMesh;
		}

		void CPUToGPUMeshSyncStage::synchronizeData(GPU::Mesh::GPUBuffer * buffer, GPUMeshList & sourceMeshes, const size_t & faceSize, const size_t & dataSize)
		{
			if (sourceMeshes.size() == 0)
			{
				return;
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