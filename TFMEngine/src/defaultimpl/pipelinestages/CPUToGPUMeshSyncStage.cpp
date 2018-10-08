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
			GPU::Mesh::GPUBuffer * staticBuffer = 
				engineInstance->getGPUMeshManager().initializeStaticBuffer();
			// Synchronize CPU -> GPU data
			synchronizeData(staticBuffer, syncOnceData);
			
			// Create dynamic buffer(s)
			// Creates both, returns front buffer
			GPU::Mesh::GPUBuffer * dynamicBuffer = 
				engineInstance->getGPUMeshManager().initializeDynamicBuffer();
			synchronizeData(dynamicBuffer, syncContinouslyData);
			
			GPU::Mesh::GPUBuffer * dynamicBackBuffer =
				engineInstance->getGPUMeshManager().getSyncDynamicBuffer();
			synchronizeData(dynamicBackBuffer, syncContinouslyData);
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
			
			GPU::Mesh::GPUBuffer * dynamicBuffer =
				engineInstance->getGPUMeshManager().getSyncDynamicBuffer();
			synchronizeData(dynamicBuffer, syncContinouslyData);
		}

		// ==============================================================================================

		void CPUToGPUMeshSyncStage::registerMeshes(
			SourceMeshList & syncOnce ,
			SourceMeshList & syncContinously)
		{
			if (elements.size() > 0)
			{
				for (auto comp : elements)
				{
					MeshRenderer * renderable = static_cast<MeshRenderer*>(comp);
					// Skips uploading mesh. If CPU_DO_NOT_SYNC, it will also overload the GPU to CPU policy
					if (renderable->cpuToGpuSync == CPUToGPUSyncPolicy::CPU_DO_NOT_SYNC)
					{
						continue;
					}

					if (renderable->cpuToGpuSync == CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING
						&& renderable->gpuToCpuSync == GPUToCPUSyncPolicy::GPU_DO_NOT_SYNC)
					{
						syncOnce.push_back(renderable);
					}
					else
					{
						syncContinously.push_back(renderable);
					}
				}

				// Clear already registered meshes, so we dont re-process them when new meshes come
				elements.clear();
			}
		}

		void CPUToGPUMeshSyncStage::createGPUMeshes(
			SourceMeshList & src, 
			GPUMeshList & dst, 
			bool staticMeshes)
		{
			size_t prevFaceSize = 0;
			for (auto mesh : src)
			{
				CPU::Mesh::Mesh * cpuMesh = mesh->getCPUMesh();
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				GPU::Mesh::GPUMesh * gpuMesh = 
					engineInstance->getGPUMeshManager().getGPUMesh(cpuMesh->index, staticMeshes);
				if (gpuMesh == NULL)
				{
					gpuMesh = buildGPUMeshFromCPUMesh(cpuMesh, prevFaceSize, staticMeshes);

					// Add each gpu mesh just once to the result, so we do not upload duplicated data to the video memory
					SyncData data;
					data.cpuMesh = cpuMesh;
					data.gpuMesh = gpuMesh;
					data.facesSize = gpuMesh->getFaceSizeGPU();
					prevFaceSize += data.facesSize;
					data.dataSize = gpuMesh->getDataSizeGPU();

					dst.push_back(data);
				}

				mesh->gpuMesh = gpuMesh;
			}
		}

		GPU::Mesh::GPUMesh * CPUToGPUMeshSyncStage::buildGPUMeshFromCPUMesh(
			CPU::Mesh::Mesh * cpuMesh, 
			size_t faceOffsetFix, 
			bool staticMesh)
		{
			GPU::Mesh::GPUMesh * gpuMesh = 
				engineInstance->getGPUMeshManager().createGPUMesh(cpuMesh->index, staticMesh);

			size_t baseOffset = cpuMesh->vertices.getMemorySource()->offset - faceOffsetFix;
			size_t vOffset = cpuMesh->vertices.getOffset();

#ifdef USE_PACKED_ATTRIB_WHEN_POSSIBLE
			unsigned int v3NumElements = 4;
			unsigned int v4NumElements = 4;
			unsigned int v2NumElements = 2;
			bool normalize = true;
			unsigned int v34DataType = GL_INT_2_10_10_10_REV;
			unsigned int v2DataType = GL_UNSIGNED_SHORT;

#else
			unsigned int v3NumElements = 3;
			unsigned int v4NumElements = 4;
			unsigned int v2NumElements = 2;
			bool normalize = false;
			unsigned int v34DataType = GL_FLOAT;
			unsigned int v2DataType = GL_FLOAT;
#endif

			gpuMesh->faces = GPU::Mesh::GPUAttribute(cpuMesh->faces.getElementSize(),
				3, 
				cpuMesh->faces.size(), 
				baseOffset + cpuMesh->faces.getOffset(), 
				cpuMesh->faces.getStride(), 
				GL_UNSIGNED_INT, 
				false);

			gpuMesh->vertices = GPU::Mesh::GPUAttribute(cpuMesh->vertices.getElementSize(),
				3, 
				cpuMesh->vertices.size(), 
				baseOffset + cpuMesh->vertices.getOffset() - vOffset,
				cpuMesh->vertices.getStride(), 
				GL_FLOAT, 
				false);

			gpuMesh->normals = GPU::Mesh::GPUAttribute(cpuMesh->normals.getElementSize(),
				v3NumElements, 
				cpuMesh->normals.size(), 
				baseOffset + cpuMesh->normals.getOffset() - vOffset,
				cpuMesh->normals.getStride(), 
				v34DataType, 
				normalize);

			gpuMesh->tangents = GPU::Mesh::GPUAttribute(cpuMesh->tangents.getElementSize(),
				v3NumElements, 
				cpuMesh->tangents.size(), 
				baseOffset + cpuMesh->tangents.getOffset() - vOffset,
				cpuMesh->tangents.getStride(), 
				v34DataType,
				normalize);

			gpuMesh->bitangents = GPU::Mesh::GPUAttribute(cpuMesh->bitangents.getElementSize(),
				v3NumElements, 
				cpuMesh->bitangents.size(), 
				baseOffset + cpuMesh->bitangents.getOffset() - vOffset,
				cpuMesh->bitangents.getStride(), 
				v34DataType,
				normalize);

			gpuMesh->uvs.resize(cpuMesh->uvs.size());
			size_t i = 0;
			for (auto & uvAttrib : cpuMesh->uvs)
			{
				gpuMesh->uvs[i] = GPU::Mesh::GPUAttribute(uvAttrib.getElementSize(),  
					v2NumElements, 
					uvAttrib.size(), 
					baseOffset + uvAttrib.getOffset() - vOffset,
					uvAttrib.getStride(), 
					v2DataType, 
					normalize);
				i++;
			}

			gpuMesh->colors.resize(cpuMesh->colors.size());
			i = 0;
			for (auto & colorAttrib : cpuMesh->colors)
			{
				gpuMesh->colors[i] = GPU::Mesh::GPUAttribute(colorAttrib.getElementSize(), 
					v4NumElements, 
					colorAttrib.size(), 
					baseOffset + colorAttrib.getOffset() - vOffset,
					colorAttrib.getStride(), 
					v34DataType,
					normalize);
				i++;
			}

			return gpuMesh;
		}

		void CPUToGPUMeshSyncStage::synchronizeData(
			GPU::Mesh::GPUBuffer * buffer, 
			GPUMeshList & sourceMeshes)
		{
			if (sourceMeshes.size() == 0)
			{
				return;
			}

			// Adjust meshes
			size_t faceSize = 0, dataSize = 0;
			for (auto syncData : sourceMeshes)
			{
				// Adjust current gpu mesh params
				syncData.gpuMesh->faceIndexOffset = faceSize;
				syncData.gpuMesh->dataIndexOffset = dataSize;
				syncData.gpuMesh->faceSize = syncData.facesSize;
				syncData.gpuMesh->dataSize = syncData.dataSize;

				// Increase global offsets
				faceSize += syncData.facesSize;
				dataSize += syncData.dataSize;
			}

			char * faceBuffer = new char[faceSize];
			char * dataBuffer = new char[dataSize];

			size_t facesOffset = 0;
			size_t dataOffset = 0;

			CPU::Memory::MemoryPool * meshPool = 
				CPU::Memory::MemoryManager::getInstance().getMemoryPool<CPU::Mesh::Mesh>();
			char * poolData = meshPool->getDataAsBytes();

			// Gather all data
			for (auto & data : sourceMeshes)
			{
				CPU::Mesh::Mesh * cpuMesh = data.cpuMesh;
				CPU::Memory::MemoryBlock * block = cpuMesh->memoryBlock;

				// Copy indexes
				memcpy(
					faceBuffer + facesOffset, poolData + block->offset, data.facesSize);
				// Copy data
				memcpy(
					dataBuffer + dataOffset, poolData + block->offset + data.facesSize, data.dataSize);

				facesOffset += data.facesSize;
				dataOffset += data.dataSize;
			}

			// Upload to GPU
			buffer->updateData(faceBuffer, faceSize, dataBuffer, dataSize);

			delete[] faceBuffer;
			delete[] dataBuffer;
		}
	}
}