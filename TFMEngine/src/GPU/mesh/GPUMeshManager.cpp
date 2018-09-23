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

				glGenVertexArrays(1, &buf->vao);
				buf->bind();
				
				glGenBuffers(1, &buf->indexBuffer);
				glGenBuffers(1, &buf->dataBuffer);

				buf->unBind();

				return buf;
			}

			GPUBuffer * GPUMeshManager::initializeDynamicBuffer()
			{
				dynamicFrontBuffer = std::make_unique<GPUBuffer>();
				GPUBuffer * buf = staticBuffer.get();
				buf->usage = GL_DYNAMIC_DRAW;

				glGenVertexArrays(1, &buf->vao);
				buf->bind();

				glGenBuffers(1, &buf->indexBuffer);
				glGenBuffers(1, &buf->dataBuffer);

				buf->unBind();

				dynamicBackBuffer = std::make_unique<GPUBuffer>();
				dynamicBackBuffer.get()->usage = GL_DYNAMIC_DRAW;
				glGenVertexArrays(1, &dynamicBackBuffer.get()->vao);
				dynamicBackBuffer.get()->bind();

				glGenBuffers(1, &dynamicBackBuffer.get()->indexBuffer);
				glGenBuffers(1, &dynamicBackBuffer.get()->dataBuffer);

				dynamicBackBuffer.get()->unBind();

				return buf;
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