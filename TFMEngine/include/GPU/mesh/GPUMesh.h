#ifndef __RENDERLIB_GPU_MESH_GPUMESH_H__
#define __RENDERLIB_GPU_MESH_GPUMESH_H__

#include <memory>
#include <vector>

#include "GPU/mesh/GPUBuffer.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			typedef struct GPUAttribute
			{
				size_t elementTypeSize;
				size_t elementCount;
				size_t numElements;
				size_t offset;
				size_t stride;

				GPUAttribute()
					: elementTypeSize(0)
					, elementCount(0)
					, numElements(0)
					, offset(0)
					, stride(0)
				{
				}

				GPUAttribute(size_t elementTypeSize, size_t elemCount, size_t numElements, size_t offset, size_t stride)
					: elementTypeSize(elementTypeSize)
					, elementCount(elemCount)
					, numElements(numElements)
					, offset(offset)
					, stride(stride)
				{
				}

			} GPUAttribute;

			class GPUMesh
			{
			public:
				GPUBuffer * gpuBuffer;
				
				size_t faceIndexOffset;
				size_t verticesBaseOffset;

				GPUAttribute faces;
				GPUAttribute vertices;
				GPUAttribute normals;
				GPUAttribute tangents;
				GPUAttribute bitangents;
				std::vector<GPUAttribute> uvs;
				std::vector<GPUAttribute> colors;

				GPUMesh();
				~GPUMesh();

				size_t getNumFaces();
				size_t getNumVertices();
			};

			typedef std::unique_ptr<GPUMesh> GPUMeshPtr;
		}
	}
}

#endif