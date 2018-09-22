#ifndef __RENDERLIB_GPU_MESH_GPUMESH_H__
#define __RENDERLIB_GPU_MESH_GPUMESH_H__

#include <memory>
#include <vector>

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			typedef struct GPUAttribute
			{
				size_t elementSize;
				size_t elementCount;
				size_t offset;
				size_t stride;

				GPUAttribute()
					: elementSize(0)
					, elementCount(0)
					, offset(0)
					, stride(0)
				{
				}

				GPUAttribute(size_t elemSize, size_t elemCount, size_t offset, size_t stride)
					: elementSize(elemSize)
					, elementCount(elemCount)
					, offset(offset)
					, stride(stride)
				{
				}

			} GPUAttribute;

			class GPUMesh
			{
			public:
				unsigned int vao;
				
				size_t faceIndexOffset;

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