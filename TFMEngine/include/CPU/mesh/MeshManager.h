#ifndef __RENDERLIB_CPU_MESHMANAGER__
#define __RENDERLIB_CPU_MESHMANAGER__

#include <map>
#include <memory>
#include <vector>

#include "CPU/memory/MemoryPool.h"

#include "CPU/mesh/AbstractMeshLoader.h"
#include "CPU/mesh/Mesh.h"

// Default mesh memory pool size 20 MB
#define DEFAULT_MESH_POOL_SIZE 20971520

namespace RenderLib
{
	namespace CPU
	{
		class MeshManager
		{
		private:
			static MeshManager * INSTANCE;
		private:
			std::map<std::string, std::vector<std::unique_ptr<Mesh>>> meshes;
			Memory::MemoryPool * meshPool;
			AbstractMeshLoader * meshLoader;
		public:
			static std::string MESH_MEMORY_POOL_NAME;

			static unsigned int OPTION_COMPUTE_NORMALS_IF_ABSENT;
			static unsigned int OPTION_COMPUTE_TANGENTS_IF_ABSENT;
			static unsigned int OPTION_COMPUTE_BITANGENTS_IF_ABSENT;
			static unsigned int OPTION_STORE_DATA_INTERLEAVED;
		public:
			static MeshManager & getInstance();
		private:
			MeshManager();
		public:
			~MeshManager();
			void setMeshPoolSize(const size_t & sizeBytes);
			void setMeshLoader(AbstractMeshLoader * meshLoader);

			Memory::MemoryPool * getMemoryPool();

			std::vector<Mesh *> && loadMeshFromFile(const std::string & file, unsigned int optionsFlag);
			std::vector<Mesh *> && getMesh(const std::string & name);

			void destroyMesh(std::vector<std::unique_ptr<Mesh>> && meshToDestroy);
			void destroy();
		private:
			std::unique_ptr<Mesh> && buildMeshFromData(MeshData * data, unsigned int optionsFlag);
		};
	}
}

#endif