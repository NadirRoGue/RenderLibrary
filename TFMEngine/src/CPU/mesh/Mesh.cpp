#include "CPU/mesh/Mesh.h"

namespace RenderLib
{
	namespace CPU
	{
		Mesh::Mesh(MeshMemoryLayout memLayout)
			: memLayout(memLayout)
		{
		}

		Mesh::Mesh(const Mesh & other)
			: memLayout(other.memLayout)
			, faces(other.faces)
			, vertices(other.vertices)
			, normals(other.normals)
			, tangents(other.tangents)
			, colors(other.colors)
			, uvs(other.uvs)
		{
		}

		Mesh::~Mesh()
		{
		}

		const MeshMemoryLayout & Mesh::getDataMemoryLayout()
		{
			return memLayout;
		}
	}
}