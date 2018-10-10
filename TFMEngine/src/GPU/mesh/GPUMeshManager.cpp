#include "GPU/mesh/GPUMeshManager.h"

#include <GL/glew.h>

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			GPUMeshManager::GPUMeshManager()
			{

			}

			GPUMeshManager::~GPUMeshManager()
			{

			}

			GPUBuffer * GPUMeshManager::initializeStaticBuffer()
			{
				staticBuffer = std::make_unique<GPUBuffer>();
				GPUBuffer * buf = staticBuffer.get();
				buf->usage = GL_STATIC_DRAW;

				buf->generate();
				
				return buf;
			}

			GPUBuffer * GPUMeshManager::initializeDynamicBuffer()
			{
				dynamicFrontBuffer = std::make_unique<GPUBuffer>();
				GPUBuffer * buf = dynamicFrontBuffer.get();
				buf->usage = GL_DYNAMIC_DRAW;

				buf->generate();

				dynamicBackBuffer = std::make_unique<GPUBuffer>();
				dynamicBackBuffer.get()->usage = GL_DYNAMIC_DRAW;
				
				dynamicBackBuffer.get()->generate();

				return buf;
			}

			GPUBuffer * GPUMeshManager::initializePostProcessQuadBuffer()
			{
				postProcessQuadBuffer = std::make_unique<GPUBuffer>();
				GPUBuffer * ppq = postProcessQuadBuffer.get();
				ppq->usage = GL_STATIC_DRAW;

				ppq->generate();

				return ppq;
			}

			GPUBuffer * GPUMeshManager::getStaticMeshBuffer()
			{
				return staticBuffer.get();
			}

			GPUBuffer * GPUMeshManager::getDynamicMeshBuffer()
			{
				return dynamicFrontBuffer.get();
			}

			GPUBuffer * GPUMeshManager::getSyncDynamicBuffer()
			{
				return dynamicBackBuffer.get();
			}

			GPUBuffer * GPUMeshManager::getPostProcessQuadBuffer()
			{
				return postProcessQuadBuffer.get();
			}

			void GPUMeshManager::swapDynamicBuffers()
			{
				dynamicFrontBuffer.swap(dynamicBackBuffer);
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
				result->index = index;
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

			GPUMesh * GPUMeshManager::getPostProcessQuad()
			{
				return postProcessQuadMesh.get();
			}
		}
	}
}