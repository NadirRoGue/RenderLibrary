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

				std::vector<IVECTOR3> loadedFaces;
				std::vector<VECTOR3> loadedVertices;
				std::vector<VECTOR3> loadedNormals;
				std::vector<VECTOR3> loadedTangents;
				std::vector<VECTOR3> loadedBitangents;
				std::vector<std::vector<VECTOR2>> loadedUvs;
				std::vector<std::vector<VECTOR4>> loadedColors;
			public:
				size_t getResultSizeBytes();
			};
		}
	}
}

#endif