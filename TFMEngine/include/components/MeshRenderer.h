#ifndef __RENDERLIB_COMPONENTS_MESHRENDERER_H__
#define __RENDERLIB_COMPONENTS_MESHRENDERER_H__

#include "Component.h"

#include "CPU/mesh/Mesh.h"
#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
	namespace Components
	{
		enum CPUToGPUSyncPolicy
		{
			CPU_SYNC_ONCE_AT_BEGINNING,
			CPU_SYNC_CONTINOUSLY,
			CPU_DO_NOT_SYNC
		};

		enum GPUToCPUSyncPolicy
		{
			GPU_SYNC_ONCE_AT_END,
			GPU_SYNC_CONTINOUSLY,
			GPU_DO_NOT_SYNC
		};

		class MeshRenderer : public Component
		{
		public:
			CPUToGPUSyncPolicy cpuToGpuSync;
			GPUToCPUSyncPolicy gpuToCpuSync;
			GPU::Mesh::GPUMesh * gpuMesh;
		public:
			MeshRenderer();
			void start();

			CPU::Mesh::Mesh * getCPUMesh();
		};
	}
}

#endif