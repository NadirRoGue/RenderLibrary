#ifndef __RENDERLIB_DEFAULTIMPL_ASSIMPFILELOADER_H__
#define __RENDERLIB_DEFAULTIMPL_ASSIMPFILELOADER_H__

#include "CPU/io/FileLoader.h"
#include "CPU/mesh/MeshLoadResult.h"

#include "CPU/mesh/Mesh.h"

#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace RenderLib
{
	namespace DefaultImpl
	{
		class AssimpFileLoader : public CPU::IO::FileLoader
		{
		public:
			AssimpFileLoader();
			CPU::IO::AbstractLoadResultPtr loadFile(const std::string & fileName, unsigned int options =
			(CPU::Mesh::Mesh::OPTION_JOIN_IDENTICAL_VERTICES 
				| CPU::Mesh::Mesh::OPTION_COMPUTE_SMOOTHNORMALS_IF_ABSENT));
		private:
			void processSceneMaterials(const aiScene * scene, CPU::Mesh::MeshLoadResult * dst, const std::string & rootPath);
			void processSceneMeshes(const aiScene * scene, CPU::Mesh::MeshLoadResult * dst, const std::string & rootPath);

			void processFileMaterial(aiMaterial * material, CPU::Mesh::MaterialLoadedData & dst, const std::string & roothPath);
			void processFileMesh(aiMesh * assimpMesh, CPU::Mesh::MeshLoadedData & dst, const std::string & rootPath);
		};
	}
}

#endif