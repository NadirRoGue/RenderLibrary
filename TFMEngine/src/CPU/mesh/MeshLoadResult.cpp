#include "CPU/mesh/MeshLoadResult.h"

#include <iostream>

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
				size += loadedNormals.size() * sizeof(VECTOR3);							// Normal size
				size += loadedTangents.size() * sizeof(VECTOR3);						// Tangent size
				size += loadedBitangents.size() * sizeof(VECTOR3);					// Bitangent size
				size += loadedUvs.size() * numVertices * sizeof(VECTOR2);		// UV Size
				size += loadedColors.size() * numVertices * sizeof(VECTOR4);// Colors size

				return size;
			}
		}
	}
}