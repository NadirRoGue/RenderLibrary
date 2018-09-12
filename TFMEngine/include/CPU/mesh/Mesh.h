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
				Memory::IVector3Attribute faces;

				Memory::Vector3Attribute vertices;
				Memory::Vector3Attribute normals;
				Memory::Vector3Attribute tangents;
				Memory::Vector3Attribute bitangents;
				std::vector<Memory::Vector2Attribute> uvs;
				std::vector<Memory::Vector4Attribute> colors;

				size_t index;
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