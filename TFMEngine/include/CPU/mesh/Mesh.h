#ifndef __RENDERLIB_CPU_MESH_H__
#define __RENDERLIB_CPU_MESH_H__

#include <vector>

#include "CPU/memory/PoolAttribute.hpp"

#include "Defines.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			class Mesh
			{
			public:
			public:
				static unsigned int OPTION_COMPUTE_NORMALS_IF_ABSENT;
				static unsigned int OPTION_COMPUTE_TANGENTS_IF_ABSENT;
				static unsigned int OPTION_COMPUTE_BITANGENTS_IF_ABSENT;
			public:
				Memory::IVector3Attribute faces;

				Memory::Vector3Attribute vertices;
#ifdef USE_PACKED_ATTRIB_WHEN_POSSIBLE
				Memory::PoolAttribute<int, 1> normals;
				Memory::PoolAttribute<int, 1> tangents;
				Memory::PoolAttribute<int, 1> bitangents;
				std::vector<Memory::PoolAttribute<unsigned short, 1>> uvs;
				std::vector<Memory::PoolAttribute<unsigned int, 1>> colors;
#else
				Memory::Vector3Attribute normals;
				Memory::Vector3Attribute tangents;
				Memory::Vector3Attribute bitangents;
				std::vector<Memory::Vector2Attribute> uvs;
				std::vector<Memory::Vector4Attribute> colors;
#endif
				size_t index;

				std::string sourceFileName;
				unsigned int materialIndex;

				Memory::MemoryBlock * memoryBlock;
			public:
				Mesh();
				Mesh(const Mesh & other);
				~Mesh();

				size_t getNumFaces();
				size_t getNumVertices();
				size_t getNumUVChannels();
				size_t getNumColorChannels();
			};
		}
	}
}

#endif