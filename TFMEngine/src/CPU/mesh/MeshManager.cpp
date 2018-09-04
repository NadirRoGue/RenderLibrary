#include "CPU/mesh/MeshManager.h"

#include "CPU/memory/MemoryPoolManager.h"

#include "CPU/mesh/meshloaders/AssimpMeshLoader.h"

namespace RenderLib
{
	namespace CPU
	{
		MeshManager * MeshManager::INSTANCE = new MeshManager();

		std::string MeshManager::MESH_MEMORY_POOL_NAME = "MeshMemoryPool";

		unsigned int MeshManager::OPTION_COMPUTE_NORMALS_IF_ABSENT = 0x01;
		unsigned int MeshManager::OPTION_COMPUTE_TANGENTS_IF_ABSENT = 0x02;
		unsigned int MeshManager::OPTION_COMPUTE_BITANGENTS_IF_ABSENT = 0x04;
		unsigned int MeshManager::OPTION_STORE_DATA_INTERLEAVED = 0x08;

		MeshManager & MeshManager::getInstance()
		{
			return *INSTANCE;
		}

		MeshManager::MeshManager()
		{
			setMeshPoolSize(DEFAULT_MESH_POOL_SIZE);
			setMeshLoader(new AssimpMeshLoader());
		}

		MeshManager::~MeshManager()
		{
			destroy();
		}

		void MeshManager::setMeshPoolSize(const size_t & sizeBytes)
		{
			Memory::MemoryPoolManager::getInstance().destroyMemoryPool(MESH_MEMORY_POOL_NAME);
			meshPool = Memory::MemoryPoolManager::getInstance().createMemoryPool(MESH_MEMORY_POOL_NAME, DEFAULT_MESH_POOL_SIZE);
		}

		void MeshManager::setMeshLoader(AbstractMeshLoader * meshLoader)
		{
			if (this->meshLoader != NULL)
			{
				delete this->meshLoader;
			}

			this->meshLoader = meshLoader;
			this->meshLoader->meshManager = this;
		}

		Memory::MemoryPool * MeshManager::getMemoryPool()
		{
			return meshPool;
		}

		std::vector<Mesh *> && MeshManager::loadMeshFromFile(const std::string & fileName, unsigned int optionsFlag)
		{
			// Check for duplicates
			auto it = meshes.find(fileName);
			if (it != meshes.end())
			{
				// Clean previous stored mesh with the given filename
				destroyMesh(std::move(it->second));
			}

			// Import meshes from file
			std::vector<MeshData*> loadedMeshes = meshLoader->loadMeshFromFile(fileName, optionsFlag);
			std::vector<Mesh*> result (loadedMeshes.size(), NULL);

			// Return a list of pointers to them
			size_t i = 0;
			for (auto meshData : loadedMeshes)
			{
				std::unique_ptr<Mesh> newMesh = buildMeshFromData(meshData, optionsFlag);
				
				result.push_back(newMesh.get());
				meshes[fileName].push_back(std::move(newMesh));
			}
			
			return std::move(result);
		}

		std::unique_ptr<Mesh> && MeshManager::buildMeshFromData(MeshData * data, unsigned int optionsFlag)
		{
			size_t sizeBytes = data->getMeshSize();

			Memory::MemoryBlock * allocatedBlock = meshPool->requestMemoryBlock(sizeBytes, true);

			MeshMemoryLayout layout = MeshMemoryLayout::MEMORY_LAYOUT_COMPACT;
			if (optionsFlag & OPTION_STORE_DATA_INTERLEAVED)
			{
				layout = MeshMemoryLayout::MEMORY_LAYOUT_INTERLEAVED;
			}

			std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>(layout);

		}

		std::vector<Mesh *> && MeshManager::getMesh(const std::string & fileName)
		{
			// Check whether mesh exist
			auto it = meshes.find(fileName);
			if (it == meshes.end())
			{
				// If not, return empty vector
				return std::move(std::vector<Mesh *>());
			}

			// Copy a pointer from all the unique ptrs
			std::vector<Mesh *> result(it->second.size());
			size_t i = 0;
			for (auto & meshPtr : it->second)
			{
				result[i++] = meshPtr.get();
			}

			return std::move(result);
		}

		void MeshManager::destroyMesh(std::vector<std::unique_ptr<Mesh>> && meshToDestroy)
		{
			for (auto & subMesh : meshToDestroy)
			{
				subMesh.reset();
			}

			meshToDestroy.clear();
		}

		void MeshManager::destroy()
		{
			auto it = meshes.begin();
			while (it != meshes.end())
			{
				destroyMesh(std::move(it->second));
				it++;
			}
		}
	}
}