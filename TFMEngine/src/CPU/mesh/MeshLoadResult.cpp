#include "CPU/mesh/MeshLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			size_t MeshLoadResult::getResultSizeBytes()
			{
				size_t size = 0;
				size += numFaces * sizeof(IVECTOR3);									// Face size
				size += numVertices * sizeof(VECTOR3);								// Vertex Size
				size += normals.size() * sizeof(VECTOR3);							// Normal size
				size += tangents.size() * sizeof(VECTOR3);						// Tangent size
				size += bitangents.size() * sizeof(VECTOR3);					// Bitangent size
				size += uvs.size() * numVertices * sizeof(VECTOR2);		// UV Size
				size += colors.size() * numVertices * sizeof(VECTOR4);// Colors size

				return size;
			}
		}
	}
}