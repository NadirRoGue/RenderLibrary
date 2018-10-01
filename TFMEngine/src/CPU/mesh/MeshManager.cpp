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
			bool compare(std::vector<IVECTOR3> & a, std::vector<IVECTOR3> & b)
			{
				if (a.size() != b.size())
				{
					return false;
				}

				for (size_t i = 0; i < a.size(); i++)
				{
					if (a[i] != b[i])
						return false;
				}

				return true;
			}

			bool compare(std::vector<VECTOR3> & a, std::vector<VECTOR3> & b)
			{
				if (a.size() != b.size())
				{
					return false;
				}

				for (size_t i = 0; i < a.size(); i++)
				{
					if (a[i] != b[i])
						return false;
				}

				return true;
			}

			bool compare(std::vector<VECTOR2> & a, std::vector<VECTOR2> & b)
			{
				if (a.size() != b.size())
				{
					return false;
				}

				for (size_t i = 0; i < a.size(); i++)
				{
					if (a[i] != b[i])
						return false;
				}

				return true;
			}

			bool compare(std::vector<VECTOR4> & a, std::vector<VECTOR4> & b)
			{
				if (a.size() != b.size())
				{
					return false;
				}

				for (size_t i = 0; i < a.size(); i++)
				{
					if (a[i] != b[i])
						return false;
				}

				return true;
			}

			void compare(MeshLoadResult * allLoaded, Mesh * inMemory)
			{
				for (auto & fromAssimp : allLoaded->loadedData)
				{
					std::vector<IVECTOR3> faces;
					std::vector<VECTOR3> vertices, normals, tangents, bitangents;
					std::vector<std::vector<VECTOR2>> uv;
					std::vector<std::vector<VECTOR4>> colors;

					inMemory->faces.dumpAttributes(faces);
					inMemory->vertices.dumpAttributes(vertices);
					inMemory->normals.dumpAttributes(normals);
					inMemory->tangents.dumpAttributes(tangents);
					inMemory->bitangents.dumpAttributes(bitangents);

					uv.resize(inMemory->uvs.size());
					for (size_t i = 0; i < inMemory->uvs.size(); i++)
					{
						inMemory->uvs[i].dumpAttributes(uv[i]);
					}

					colors.resize(inMemory->colors.size());
					for (size_t i = 0; i < inMemory->colors.size(); i++)
					{
						inMemory->colors[i].dumpAttributes(colors[i]);
					}

					if (!compare(fromAssimp.loadedFaces, faces))
						std::cout << "Faces missmatch" << std::endl;

					if (!compare(fromAssimp.loadedVertices, vertices))
						std::cout << "Vertices missmatch" << std::endl;

					if (!compare(fromAssimp.loadedNormals, normals))
						std::cout << "Normals missmatch" << std::endl;

					if (!compare(fromAssimp.loadedTangents, tangents))
						std::cout << "Tangents missmatch" << std::endl;

					for (size_t i = 0; i < fromAssimp.numUVMaps; i++)
					{
						if (!compare(fromAssimp.loadedUvs[i], uv[i]))
							std::cout << "UVs " << i << " missmatchs" << std::endl;
					}

					for (size_t i = 0; i < fromAssimp.numColorLayers; i++)
					{
						if (!compare(fromAssimp.loadedColors[i], colors[i]))
							std::cout << "Colors " << i << " missmatch" << std::endl;
					}
				}
			}

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
				// Import data from file
				IO::AbstractLoadResultPtr loadedMeshes = IO::FileManager::loadFile(fileName, optionsFlag);
				MeshLoadResult * result = static_cast<MeshLoadResult*>(loadedMeshes.get());

				// Process meshes
				std::vector<Mesh*> meshList = processLoadedMeshes(fileName, result->loadedData);

				// Process materials
				

				return meshList;
			}

			std::vector<Mesh *> MeshManager::processLoadedMeshes(const std::string & fileName, std::vector<MeshLoadedData> & loadedData)
			{
				// Check for duplicates
				auto it = meshes.find(fileName);
				if (it != meshes.end())
				{
					// Clean previous stored mesh with the given filename
					destroyMesh(std::move(it->second));
				}

				std::vector<std::unique_ptr<Mesh>> storageResult;
				storageResult.reserve(loadedData.size());
				std::vector<Mesh *> result;
				result.reserve(loadedData.size());

				for (auto & meshData : loadedData)
				{
					std::unique_ptr<Mesh> newMesh = buildMeshFromData(meshData, 0);
					result.push_back(newMesh.get());
					storageResult.push_back(std::move(newMesh));
				}

				meshes[fileName] = std::move(storageResult);

				return result;
			}

			std::unique_ptr<Mesh> MeshManager::buildMeshFromData(MeshLoadedData & data, unsigned int optionsFlag)
			{
				// Create new object
				std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>();
				Mesh * meshPtr = newMesh.get();

				MeshBlockConfiguration memConfig;
				memConfig.numFaces = data.numFaces;
				memConfig.numVertices = data.numVertices;
				memConfig.hasNormals = data.loadedNormals.size() == data.numVertices;
				memConfig.hasTangents = data.loadedTangents.size() == data.numVertices;
				memConfig.hasBiTangents = data.loadedBitangents.size() == data.numVertices;
				memConfig.numUVChannels = data.loadedUvs.size();
				memConfig.numColorChannels = data.loadedColors.size();
				// Configure memory pool attributes
				Memory::MemoryManager::getInstance().configureObject<Mesh>(newMesh.get(), &memConfig);

				// Copy data to pool
				meshPtr->faces.setAttributes(data.loadedFaces);
				meshPtr->vertices.setAttributes(data.loadedVertices);
				meshPtr->normals.setAttributes(data.loadedNormals);
				meshPtr->tangents.setAttributes(data.loadedTangents);
				meshPtr->bitangents.setAttributes(data.loadedBitangents);
				
				for (size_t i = 0; i < data.numUVMaps; i++)
				{
					meshPtr->uvs[i].setAttributes(data.loadedUvs[i]);
				}

				for (size_t i = 0; i < data.numColorLayers; i++)
				{
					meshPtr->colors[i].setAttributes(data.loadedColors[i]);
				}

				//compare(data, meshPtr);

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