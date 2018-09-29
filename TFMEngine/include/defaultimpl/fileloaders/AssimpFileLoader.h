#ifndef __RENDERLIB_DEFAULTIMPL_ASSIMPFILELOADER_H__
#define __RENDERLIB_DEFAULTIMPL_ASSIMPFILELOADER_H__

#include "CPU/io/FileLoader.h"
#include "CPU/mesh/MeshLoadResult.h"

#include <assimp/mesh.h>

namespace RenderLib
{
	namespace DefaultImpl
	{
		class AssimpFileLoader : public CPU::IO::FileLoader
		{
		public:
			AssimpFileLoader();
			std::vector<CPU::IO::AbstractLoadResultPtr> loadFile(const std::string & fileName, unsigned int options);
		private:
			std::unique_ptr<CPU::Mesh::MeshLoadResult> processFileMesh(aiMesh * assimpMesh, unsigned int options);
		};
	}
}

#endif