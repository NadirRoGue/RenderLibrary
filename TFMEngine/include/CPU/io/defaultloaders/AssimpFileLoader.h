#ifndef __RENDERLIB_CPU_ASSIMPFILELOADER_H__
#define __RENDERLIB_CPU_ASSIMPFILELOADER_H__

#include "CPU/io/FileLoader.h"
#include "CPU/mesh/MeshLoadResult.h"

#include <assimp/mesh.h>

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class AssimpFileLoader : public FileLoader
			{
			public:
				AssimpFileLoader();
				std::vector<AbstractLoadResult*> loadFile(const std::string & fileName, unsigned int options);
			private:
				Mesh::MeshLoadResult * processFileMesh(aiMesh * assimpMesh, unsigned int options);
			};
		}
	}
}

#endif