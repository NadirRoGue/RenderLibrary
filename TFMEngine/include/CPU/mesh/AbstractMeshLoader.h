#ifndef __RENDERLIB_CPU_ABSTRACTMESHLOADER_H__
#define __RENDERLIB_CPU_ABSTRACTMESHLOADER_H__

#include <memory>
#include <string>
#include <vector>

#include "CPU/mesh/Mesh.h"

#include "Defines.h"

namespace RenderLib
{
	namespace CPU
	{
		class MeshManager;

		typedef struct MeshData
		{
			size_t numFaces;
			size_t numVertices;

			size_t numUVMaps;
			size_t numColorLayers;

			std::vector<IVECTOR3> faces;
			std::vector<VECTOR3> vertices;
			std::vector<VECTOR3> normals;
			std::vector<VECTOR3> tangents;
			std::vector<VECTOR3> bitangents;
			std::vector<std::vector<VECTOR2>> uvs;
			std::vector<std::vector<VECTOR4>> colors;

			size_t && getMeshSize()
			{
				size_t size = 0;
				size += numFaces * sizeof(IVECTOR3);									// Face size
				size += numVertices * sizeof(VECTOR3);								// Vertex Size
				size += normals.size() * sizeof(VECTOR3);							// Normal size
				size += tangents.size() * sizeof(VECTOR3);						// Tangent size
				size += bitangents.size() * sizeof(VECTOR3);					// Bitangent size
				size += uvs.size() * numVertices * sizeof(VECTOR2);		// UV Size
				size += colors.size() * numVertices * sizeof(VECTOR4);// Colors size

				return std::move(size);
			}

		} MeshData;

		class AbstractMeshLoader
		{
		public:
			MeshManager * meshManager;
		public:
			virtual std::vector<MeshData*> && loadMeshFromFile(std::string fileName, unsigned int options) = 0;
		};
	}
}

#endif