#include "CPU/mesh/MeshManager.h"

#include <stdexcept>

#include "CPU/io/FileManager.h"
#include "CPU/memory/MemoryManager.h"
#include "CPU/mesh/MeshBlockConfiguration.h"

#include <iostream>

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			MeshManager * MeshManager::INSTANCE = new MeshManager();

			unsigned int MeshManager::OPTION_COMPUTE_NORMALS_IF_ABSENT = 0x01;
			unsigned int MeshManager::OPTION_COMPUTE_TANGENTS_IF_ABSENT = 0x02;
			unsigned int MeshManager::OPTION_COMPUTE_BITANGENTS_IF_ABSENT = 0x04;

			MeshManager & MeshManager::getInstance()
			{
				return *INSTANCE;
			}

			MeshManager::MeshManager()
			{
			}

			MeshManager::~MeshManager()
			{
				destroy();
			}

			std::vector<Mesh *> MeshManager::loadMeshFromFile(const std::string & fileName, unsigned int optionsFlag)
			{
				// Check for duplicates
				auto it = meshes.find(fileName);
				if (it != meshes.end())
				{
					// Clean previous stored mesh with the given filename
					destroyMesh(std::move(it->second));
				}

				// Import meshes from file
				std::vector<IO::AbstractLoadResult *> loadedMeshes = IO::FileManager::loadFile(fileName, optionsFlag);
				std::vector<Mesh*> result;
				result.reserve(loadedMeshes.size());

				// Return a list of pointers to them
				size_t i = 0;
				for (auto meshData : loadedMeshes)
				{
					MeshLoadResult * meshLoadResult = dynamic_cast<MeshLoadResult*>(meshData);
					if (meshLoadResult)
					{
						std::unique_ptr<Mesh> newMesh = buildMeshFromData(meshLoadResult, optionsFlag);

						result.push_back(newMesh.get());
						meshes[fileName].push_back(std::move(newMesh));
					}
				}

				return result;
			}

			std::unique_ptr<Mesh> MeshManager::buildMeshFromData(MeshLoadResult * data, unsigned int optionsFlag)
			{
				// Create new object
				std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>();
				Mesh * meshPtr = newMesh.get();

				MeshBlockConfiguration memConfig;
				memConfig.numFaces = data->numFaces;
				memConfig.numVertices = data->numVertices;
				memConfig.hasNormals = data->normals.size() == data->numVertices;
				memConfig.hasTangents = data->tangents.size() == data->numVertices;
				memConfig.hasBiTangents = data->bitangents.size() == data->numVertices;
				memConfig.numUVChannels = data->uvs.size();
				memConfig.numColorChannels = data->colors.size();
				// Configure memory pool attributes
				Memory::MemoryManager::getInstance().configureObject<Mesh>(newMesh.get(), &memConfig);

				// Copy data to pool
				meshPtr->faces.setAttributes(data->faces);
				std::cout << "Copied faces" << std::endl;
				meshPtr->vertices.setAttributes(data->vertices);
				std::cout << "Copied vertices" << std::endl;
				meshPtr->normals.setAttributes(data->normals);
				std::cout << "Copied normals" << std::endl;
				meshPtr->tangents.setAttributes(data->tangents);
				std::cout << "Copied tangents" << std::endl;
				meshPtr->bitangents.setAttributes(data->bitangents);
				std::cout << "Copied bitangents" << std::endl;

				for (int i = 0; i < data->numUVMaps; i++)
				{
					meshPtr->uvs[i].setAttributes(data->uvs[i]);
				}

				for (int i = 0; i < data->numColorLayers; i++)
				{
					meshPtr->colors[i].setAttributes(data->colors[i]);
				}

				return newMesh;
			}

			std::vector<Mesh *> MeshManager::getMesh(const std::string & fileName)
			{
				// Check whether mesh exist
				auto it = meshes.find(fileName);
				if (it == meshes.end())
				{
					// If not, return empty vector
					return std::vector<Mesh *>();
				}

				// Copy a pointer from all the unique ptrs
				std::vector<Mesh *> result(it->second.size());
				size_t i = 0;
				for (auto & meshPtr : it->second)
				{
					result[i++] = meshPtr.get();
				}

				return result;
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
}