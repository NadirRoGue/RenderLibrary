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
				for (auto data : loadedData)
				{
					size += data.numFaces * sizeof(IVECTOR3);								// Face size
					size += data.numVertices * sizeof(VECTOR3);								// Vertex Size
					size += data.loadedNormals.size() * sizeof(VECTOR3);					// Normal size
					size += data.loadedTangents.size() * sizeof(VECTOR3);					// Tangent size
					size += data.loadedBitangents.size() * sizeof(VECTOR3);					// Bitangent size
					size += data.loadedUvs.size() * data.numVertices * sizeof(VECTOR2);		// UV Size
					size += data.loadedColors.size() * data.numVertices * sizeof(VECTOR4);	// Colors size
				}

				return size;
			}
		}
	}
}