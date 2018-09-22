#ifndef __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__
#define __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__

#include <memory>
#include <map>

#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			class GPUMeshManager
			{
			private:
				static std::unique_ptr<GPUMeshManager> INSTANCE;
			public:
				static GPUMeshManager & getInstance();
			private:
				std::map<size_t, GPUMeshPtr> staticMeshes;
				std::map<size_t, GPUMeshPtr> dynamicMeshes;
			public:
				GPUMeshManager();
				~GPUMeshManager();

				bool meshAlreadyExists(size_t index, bool staticMesh);
				GPUMesh * getGPUMesh(size_t index, bool staticMesh);
				GPUMesh * createGPUMesh(size_t index, bool staticMesh);
			};
		}
	}
}

#endif