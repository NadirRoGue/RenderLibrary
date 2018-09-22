#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
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
		}
	}
}