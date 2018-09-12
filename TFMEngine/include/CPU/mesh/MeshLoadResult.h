#ifndef __RENDERLIB_CPU_MESHLOADRESULT_H__
#define __RENDERLIB_CPU_MESHLOADRESULT_H__

#include <vector>

#include "Defines.h"
#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			class MeshLoadResult : public IO::AbstractLoadResult
			{
			public:
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

				size_t getResultSizeBytes();
			};
		}
	}
}

#endif