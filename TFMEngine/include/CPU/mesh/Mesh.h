#ifndef __RENDERLIB_CPU_MESH_H__
#define __RENDERLIB_CPU_MESH_H__

#include <vector>

#include "CPU/memory/PoolAttribute.hpp"

#include "Defines.h"

namespace RenderLib
{
	namespace CPU
	{
		enum MeshMemoryLayout
		{
			MEMORY_LAYOUT_COMPACT,
			MEMORY_LAYOUT_INTERLEAVED
		};

		class Mesh
		{
		private:
			MeshMemoryLayout memLayout;
		public:
			Memory::IVector3Attribute faces;

			Memory::Vector3Attribute vertices;
			Memory::Vector3Attribute normals;
			Memory::Vector3Attribute tangents;
			Memory::Vector3Attribute bitangents;
			Memory::Vector2Attribute uvs [MAX_UV_MAPS_PER_MESH];
			Memory::Vector4Attribute colors [MAX_COLOR_LAYER_PER_MESH];

		public:
			Mesh(MeshMemoryLayout memLayout);
			Mesh(const Mesh & other);
			~Mesh();

			const MeshMemoryLayout & getDataMemoryLayout();
		};

		class MeshWrapper
		{
		public:
			std::vector<Mesh*> meshes;
		};
	}
}

#endif