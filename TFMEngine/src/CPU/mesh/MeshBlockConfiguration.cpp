#include "CPU/mesh/MeshBlockConfiguration.h"

#include "Defines.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			size_t MeshBlockConfiguration::getSizeBytes()
			{
				size_t result = 0;

				result += numFaces * sizeof(IVECTOR3);
				result += numVertices * sizeof(VECTOR3);
				
				result += numVertices * sizeof(VECTOR3) * (hasNormals ? 1 : 0);
				result += numVertices * sizeof(VECTOR3) * (hasTangents ? 1 : 0);
				result += numVertices * sizeof(VECTOR3) * (hasBiTangents ? 1 : 0);
				result += numUVChannels * numVertices * sizeof(VECTOR2);
				result += numColorChannels * numVertices * sizeof(VECTOR4);

				return result;
			}
		}
	}
}