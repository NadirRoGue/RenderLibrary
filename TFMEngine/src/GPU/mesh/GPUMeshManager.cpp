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

			void GPUMeshManager::generatePostProcessQuad()
			{
				unsigned int faces[] =
				{
					0,1,2,
					1,3,2
				};

				float verticesAndUvs[] = 
				{
					-1.0f,	-1.0f,	 0.0f,
					0.0f, 0.0f,
					1.0f,	-1.0f,	 0.0f,
					1.0f, 0.0f,
					-1.0f,	 1.0f,	 0.0f,
					0.0f, 1.0f,
					1.0f,	 1.0f,	 0.0f,
					1.0f, 1.0f
				};
				/*
				const float uvs[] = {
					0.0f, 0.0f,
					1.0f, 0.0f,
					0.0f, 1.0f,
					1.0f, 1.0f
				};
				*/
				initializePostProcessQuadBuffer();
				GPUBuffer * ppqb = postProcessQuadBuffer.get();
				ppqb->updateData(
					reinterpret_cast<char*>(faces), 
					sizeof(faces), 
					reinterpret_cast<char*>(verticesAndUvs), 
					sizeof(verticesAndUvs));

				postProcessQuadMesh = std::make_unique<GPUMesh>();
				GPUMesh * ppqm = postProcessQuadMesh.get();

				ppqm->faces = GPUAttribute(
					sizeof(unsigned int) * 3,	// One face size in bytes
					3,							// Num indices per face
					2,							// Num faces
					0,							// Offset
					0,							// Stride
					GL_UNSIGNED_INT,			// Data type (Used for other attribs)		
					false);						// Normalize on upload (Used for other attribs)

				ppqm->vertices = GPUAttribute(
					sizeof(float) * 3,
					3, 
					4, 
					0, 
					sizeof(float) * 5, // Stride: 3 vertex coords + 2 uv coords
					GL_FLOAT, 
					false);

				ppqm->uvs.resize(1);
				ppqm->uvs[0] = GPUAttribute(
					sizeof(float) * 2,
					2,
					4,
					sizeof(float) * 3,
					sizeof(float) * 5,
					GL_FLOAT,
					false);
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