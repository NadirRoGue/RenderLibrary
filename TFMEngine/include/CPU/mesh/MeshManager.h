#ifndef __RENDERLIB_CPU_MESHMANAGER__
#define __RENDERLIB_CPU_MESHMANAGER__

#include <unordered_map>
#include <memory>
#include <vector>

#include "Defines.h"

#include "CPU/mesh/Mesh.h"
#include "CPU/mesh/MeshLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			class MeshManager
			{
			private:
				static MeshManager * INSTANCE;
			private:
				std::unordered_map<std::string, std::vector<std::unique_ptr<Mesh>>> meshes;
			public:
				static unsigned int OPTION_COMPUTE_NORMALS_IF_ABSENT;
				static unsigned int OPTION_COMPUTE_TANGENTS_IF_ABSENT;
				static unsigned int OPTION_COMPUTE_BITANGENTS_IF_ABSENT;
			public:
				static MeshManager & getInstance();
			private:
				MeshManager();
			public:
				~MeshManager();

				std::vector<Mesh *> loadMeshFromFile(const std::string & file, unsigned int optionsFlag);
				std::vector<Mesh *> getMesh(const std::string & name);

				void destroyMesh(std::vector<std::unique_ptr<Mesh>> && meshToDestroy);
				void destroy();
			private:
				std::vector<Mesh *> processLoadedMeshes(const std::string & filename, std::vector<MeshLoadedData> & loadedMeshes);
				std::unique_ptr<Mesh> buildMeshFromData(MeshLoadedData & data, unsigned int optionsFlag);
				
			};
		}
	}
}

#endif