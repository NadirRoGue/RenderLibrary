#ifndef __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__
#define __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__

#include <memory>
#include <map>

#include "GPU/mesh/GPUMesh.h"
#include "GPU/mesh/GPUBuffer.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			class GPUMeshManager
			{
			private:
				std::map<size_t, GPUMeshPtr> staticMeshes;
				std::map<size_t, GPUMeshPtr> dynamicMeshes;

				GPUBufferPtr staticBuffer;

				GPUBufferPtr dynamicFrontBuffer;
				GPUBufferPtr dynamicBackBuffer;
			public:
				GPUMeshManager();
				~GPUMeshManager();

				GPUBuffer * initializeStaticBuffer();
				GPUBuffer * initializeDynamicBuffer();

				void swapDynamicBuffers();

				bool meshAlreadyExists(size_t index, bool staticMesh);
				GPUMesh * getGPUMesh(size_t index, bool staticMesh);
				GPUMesh * createGPUMesh(size_t index, bool staticMesh);
			};
		}
	}
}

#endif