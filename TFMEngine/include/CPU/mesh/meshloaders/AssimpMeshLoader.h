#ifndef __RENDERLIB_CPU_ASSIMPMESHLOADER_H__
#define __RENDERLIB_CPU_ASSIMPMESHLOADER_H__

#include "CPU/mesh/AbstractMeshLoader.h"

#include <assimp/mesh.h>

namespace RenderLib
{
	namespace CPU
	{
		class AssimpMeshLoader : public AbstractMeshLoader
		{
		public:
			std::vector<MeshData*> && loadMeshFromFile(std::string fileName, unsigned int options);
		private:
			MeshData * processFileMesh(aiMesh * assimpMesh, unsigned int options);
		};
	}
}

#endif