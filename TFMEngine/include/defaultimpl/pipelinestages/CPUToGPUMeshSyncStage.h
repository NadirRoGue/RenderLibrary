#ifndef __RENDERLIB_DEFAULTIMPL_CPUTOGPUMESHSYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_CPUTOGPUMESHSYNCSTAGE_H__

#include <vector>

#include "defaultimpl/components/MeshRenderer.h"

#include "pipeline/PipelineStage.h"

#include "GPU/mesh/GPUMesh.h"
#include "GPU/mesh/GPUBuffer.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		typedef struct SyncData
		{
			CPU::Mesh::Mesh * cpuMesh;
			GPU::Mesh::GPUMesh * gpuMesh;
			size_t facesSize;
			size_t dataSize;

		} SyncData;

		typedef std::vector<DefaultImpl::MeshRenderer *> SourceMeshList;
		typedef std::vector<SyncData> GPUMeshList;

		class CPUToGPUMeshSyncStage : public Pipeline::ElementBasedStage<DefaultImpl::MeshRenderer>
		{
		private:
			SourceMeshList syncOnce;
			SourceMeshList syncContinously;
		public:
			void preRunStage();
			void runStage();
		private:
			void registerMeshes();
			void createGPUMeshes(SourceMeshList & src, GPUMeshList & dst, size_t & faceSizeBytes, size_t & dataSizeBytes, bool staticMeshes);
			void synchronizeData(GPU::Mesh::GPUBuffer * buffer, GPUMeshList & sourceMeshes, const size_t & faceSize, const size_t & dataSize);
			GPU::Mesh::GPUMesh * buildGPUMeshFromCPUMesh(CPU::Mesh::Mesh * cpuMesh, bool staticMesh);
		};
	}
}

#endif