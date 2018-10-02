#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			inline size_t GPUMeshAttributeSize(const GPUAttribute & attrib)
			{
				return attrib.elementTypeSize * attrib.numElements;
			}

			GPUMesh::GPUMesh()
			{

			}

			GPUMesh::~GPUMesh()
			{

			}

			size_t GPUMesh::getNumFaces()
			{
				return faces.elementCount;
			}

			size_t GPUMesh::getNumVertices()
			{
				return vertices.elementCount;
			}

			size_t GPUMesh::getFaceSizeGPU()
			{
				return GPUMeshAttributeSize(faces);
			}

			size_t GPUMesh::getDataSizeGPU()
			{
				size_t size = 0;
				size += GPUMeshAttributeSize(vertices);
				size += GPUMeshAttributeSize(normals);
				size += GPUMeshAttributeSize(tangents);
				size += GPUMeshAttributeSize(bitangents);

				if (uvs.size() > 0)
				{
					size += uvs.size() * GPUMeshAttributeSize(uvs[0]);
				}

				if (colors.size() > 0)
				{
					size += colors.size() * GPUMeshAttributeSize(colors[0]);
				}

				return size;
			}
		}
	}
}