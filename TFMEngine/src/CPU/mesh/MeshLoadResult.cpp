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
				for (auto data : loadedData)
				{
					size += data.numFaces * sizeof(IVECTOR3);								
					size += data.numVertices * sizeof(VECTOR3);								
					size += data.loadedNormals.size() * sizeof(VECTOR3);					
					size += data.loadedTangents.size() * sizeof(VECTOR3);					
					size += data.loadedBitangents.size() * sizeof(VECTOR3);				
					size += data.loadedUvs.size() * data.numVertices * sizeof(VECTOR2);		
					size += data.loadedColors.size() * data.numVertices * sizeof(VECTOR4);
				}

				return size;
			}
		}
	}
}