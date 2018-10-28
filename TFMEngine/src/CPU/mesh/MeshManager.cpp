#include "CPU/mesh/MeshManager.h"

#include <stdexcept>

#include "CPU/io/FileManager.h"
#include "CPU/memory/MemoryManager.h"
#include "CPU/mesh/MeshBlockConfiguration.h"

#include "material/MaterialManager.h"

#include "CPU/mesh/MeshUtil.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			inline void packVector4ToUint(
				std::vector<VECTOR4> & src, 
				std::vector<unsigned int> & dst)
			{
				dst.reserve(src.size());
				for (auto & v : src)
				{
					VECTOR4 normalized = v;// .normalized();
					unsigned int buf;

					const int16_t maxValue = static_cast<int16_t>(
						std::numeric_limits<uint8_t>::max());
					const int16_t negativeValueScale = maxValue + 1;

					buf |= static_cast<uint8_t>(normalized.x() < 0? normalized.x() * 
						negativeValueScale : normalized.x() * maxValue);
					buf |= static_cast<uint8_t>(normalized.y() < 0? normalized.y() * 
						negativeValueScale : normalized.y() * maxValue) << 8;
					buf |= static_cast<uint8_t>(normalized.z() < 0? normalized.z() * 
						negativeValueScale : normalized.z() * maxValue) << 16;
					buf |= static_cast<uint8_t>(normalized.w() < 0? normalized.w() * 
						negativeValueScale : normalized.w() * maxValue) << 24;
					dst.push_back(buf);
				}
			}

			inline void packVector3ToUInt(
				std::vector<VECTOR3> & src, 
				std::vector<int> & dst)
			{
				int w = 0;
				dst.reserve(src.size());
				for (auto & v : src)
				{
					VECTOR3 normalized = v.normalized();
					int buf = 0;

					const int8_t maxValue = (std::numeric_limits<int8_t>::max());
					const int8_t negativeValueScale = maxValue;

					int a = static_cast<int>(normalized.x() * 127.0) & 0x000000ff;
					int b = static_cast<int>(normalized.y() * 127.0) & 0x000000ff;
					int c = static_cast<int>(normalized.z() * 127.0) & 0x000000ff;
					int d = 0;

					buf |= (d << 30);
					buf |= (c << 20);
					buf |= (b << 10);
					buf |= a;

					dst.push_back(buf);
				}
			}

			inline void packVector2ToUShort(
				std::vector<VECTOR2> & src, 
				std::vector<unsigned short> & dst)
			{
				dst.reserve(src.size());
				for (auto & v : src)
				{
					VECTOR2 normalized = v.normalized();
					unsigned short buf;
					buf |= static_cast<int8_t>(normalized.x());
					buf |= static_cast<int8_t>(normalized.y()) << 8;
					dst.push_back(buf);
				}
			}

			MeshManager MeshManager::INSTANCE;

			MeshManager & MeshManager::getInstance()
			{
				return INSTANCE;
			}

			MeshManager::MeshManager()
			{
			}

			MeshManager::~MeshManager()
			{
				destroy();
			}

			std::vector<Mesh *> MeshManager::loadMeshFromFile(
				const std::string & fileName, 
				unsigned int optionsFlag)
			{
				std::unique_lock<std::mutex> lock(mtx);

				// Import data from file
				IO::AbstractLoadResultPtr loadedMeshes = 
					IO::FileManager::loadFile(fileName, optionsFlag);
				MeshLoadResult * result = 
					static_cast<MeshLoadResult*>(loadedMeshes.get());

				// Process meshes
				std::vector<Mesh*> meshList = 
					processLoadedMeshes(fileName, result->loadedData);

				// Process materials
				Material::MaterialManager::getInstance()
					.parseMaterials(fileName, result->loadedMaterials);

				lock.unlock();

				return meshList;
			}

			std::vector<Mesh *> MeshManager::processLoadedMeshes(
				const std::string & fileName, 
				std::vector<MeshLoadedData> & loadedData)
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

			std::unique_ptr<Mesh> MeshManager::buildMeshFromData(
				MeshLoadedData & data, 
				unsigned int optionsFlag)
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
#ifdef USE_PACKED_ATTRIB_WHEN_POSSIBLE
				std::vector<int> packedNormals, packedTangents, packedBitangents;
				std::vector<std::vector<unsigned int>> packedColors;
				std::vector<std::vector<unsigned short>> packedUvs;

				packVector3ToUInt(data.loadedNormals, packedNormals);
				packVector3ToUInt(data.loadedTangents, packedTangents);
				packVector3ToUInt(data.loadedBitangents, packedBitangents);

				packedUvs.resize(data.numUVMaps);
				for (size_t i = 0; i < data.numUVMaps; i++)
				{
					packVector2ToUShort(data.loadedUvs[i], packedUvs[i]);
				}
				packedColors.resize(data.numColorLayers);
				for (size_t i = 0; i < data.numColorLayers; i++)
				{
					packVector4ToUint(data.loadedColors[i], packedColors[i]);
				}

				meshPtr->normals.setAttributes(packedNormals);
				meshPtr->tangents.setAttributes(packedTangents);
				meshPtr->bitangents.setAttributes(packedBitangents);
				for (size_t i = 0; i < data.numUVMaps; i++)
				{
					meshPtr->uvs[i].setAttributes(packedUvs[i]);
				}
				for (size_t i = 0; i < data.numColorLayers; i++)
				{
					meshPtr->colors[i].setAttributes(packedColors[i]);
				}
#else
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
#endif
				//compare(data, meshPtr);

				meshPtr->materialIndex = data.materialIndex;
				meshPtr->sourceFileName = data.srcFile;

				//ComputeNormals(meshPtr);
				//ComputeTangents(meshPtr);

				return newMesh;
			}

			std::vector<Mesh *> MeshManager::getMesh(
				const std::string & fileName)
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

			void MeshManager::destroyMesh(
				std::vector<std::unique_ptr<Mesh>> && meshToDestroy)
			{
				std::unique_lock<std::mutex> lock(mtx);

				for (auto & subMesh : meshToDestroy)
				{
					subMesh.reset();
				}

				meshToDestroy.clear();

				lock.unlock();
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