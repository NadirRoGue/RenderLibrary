#include "GPU/mesh/GPUMeshManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			std::unique_ptr<GPUMeshManager> GPUMeshManager::INSTANCE = std::make_unique<GPUMeshManager>();

			GPUMeshManager & GPUMeshManager::getInstance()
			{
				return *(INSTANCE.get());
			}

			GPUMeshManager::GPUMeshManager()
			{

			}

			GPUMeshManager::~GPUMeshManager()
			{

			}

			bool GPUMeshManager::meshAlreadyExists(size_t index, bool staticMesh)
			{
				return getGPUMesh(index, staticMesh) != NULL;
			}

			GPUMesh * GPUMeshManager::getGPUMesh(size_t index, bool staticMesh)
			{
				if (staticMesh)
				{
					auto it = staticMeshes.find(index);
					if (it != staticMeshes.end())
					{
						return it->second.get();
					}
				}
				else
				{
					auto it = dynamicMeshes.find(index);
					if (it != dynamicMeshes.end())
					{
						return it->second.get();
					}
				}

				return NULL;
			}

			GPUMesh * GPUMeshManager::createGPUMesh(size_t index, bool staticMesh)
			{
				GPUMeshPtr newGPUMesh = std::make_unique<GPUMesh>();
				GPUMesh * result = newGPUMesh.get();
				if (staticMesh)
				{
					staticMeshes[index] = std::move(newGPUMesh);
				}
				else
				{
					dynamicMeshes[index] = std::move(newGPUMesh);
				}
				return result;
			}
		}
	}
}