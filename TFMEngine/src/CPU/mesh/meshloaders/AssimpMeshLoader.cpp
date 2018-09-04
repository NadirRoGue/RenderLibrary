#include "CPU/mesh/meshloaders/AssimpMeshLoader.h"

#include "Defines.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <iostream>

#include "CPU/mesh/MeshManager.h"

namespace RenderLib
{
	namespace CPU
	{
		inline VECTOR2 && assimpToEigen2F(const aiVector2D & source)
		{
			return std::move(VECTOR2(source.x, source.y));
		}

		inline VECTOR3 && assimpToEigen3F(const aiVector3D & source)
		{
			return std::move(VECTOR3(source.x, source.y, source.z));
		}

		inline VECTOR4 && assimpToEigen4F(const aiColor4D & source)
		{
			return std::move(VECTOR4(source.r, source.g, source.b, source.a));
		}

		inline IVECTOR3 && assimpToEigen3I(const aiFace & face)
		{
			return std::move(IVECTOR3(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
		}

		std::vector<MeshData*> && AssimpMeshLoader::loadMeshFromFile(std::string fileName, unsigned int options)
		{
			Assimp::Importer importer;

			unsigned int assimpFlags = 0;

			assimpFlags |= options & MeshManager::OPTION_COMPUTE_NORMALS_IF_ABSENT ? aiProcess_GenNormals : 0;
			assimpFlags |= (options & MeshManager::OPTION_COMPUTE_TANGENTS_IF_ABSENT
										 || options & MeshManager::OPTION_COMPUTE_BITANGENTS_IF_ABSENT) ? aiProcess_CalcTangentSpace : 0;
			assimpFlags |= aiProcess_Triangulate;

			const aiScene * scene = importer.ReadFile(fileName, assimpFlags);

			if (scene == NULL)
			{
				std::string importerError(importer.GetErrorString());
				std::string errorMessage = "AssimpMeshLoader: Error importing mesh " + fileName + ": " + importerError;
				throw std::runtime_error(errorMessage);
			}

			std::vector<MeshData*> result;
			size_t numMeshes = scene->mNumMeshes;
			size_t i = 0;
			while (i < numMeshes)
			{
				aiMesh * meshToProcess = scene->mMeshes[i];
				MeshData * engineMesh = processFileMesh(meshToProcess, options);

				if (engineMesh != NULL)
				{
					result.push_back(engineMesh);
				}
				else
				{
					std::cerr << "AssimpMeshLoader: Error while processing mesh " << i << " from file " << fileName << ". Skipping." << std::endl;
				}

				i++;
			}

			return std::move(result);
		}

		MeshData * AssimpMeshLoader::processFileMesh(aiMesh * mesh, unsigned int options)
		{
			MeshData * meshData = new MeshData;

			size_t i = 0;

			meshData->numFaces = mesh->mNumFaces;
			meshData->faces.reserve(meshData->numFaces);
			while (i < meshData->numFaces)
			{
				aiFace & face = mesh->mFaces[i++];
				meshData->faces.push_back(assimpToEigen3I(face));
			}

			i = 0;

			if (mesh->mNumVertices > 0)
			{
				meshData->numVertices = mesh->mNumVertices;
				meshData->vertices.reserve(meshData->numVertices);
				while (i < meshData->numVertices)
				{
					meshData->vertices.push_back(assimpToEigen3F(mesh->mVertices[i]));
				}

				if (mesh->mNormals != NULL)
				{
					i = 0;
					meshData->normals.reserve(meshData->numVertices);
					while (i < meshData->numVertices)
					{
						meshData->normals.push_back(assimpToEigen3F(mesh->mNormals[i]));
					}
				}

				if (mesh->mTangents != NULL)
				{
					meshData->tangents.reserve(meshData->numVertices);
					while (i < meshData->numVertices)
					{
						meshData->tangents.push_back(assimpToEigen3F(mesh->mTangents[i]));
					}
				}

				if (mesh->mBitangents != NULL)
				{
					meshData->bitangents.reserve(meshData->numVertices);
					while (i < meshData->numVertices)
					{
						meshData->bitangents.push_back(assimpToEigen3F(mesh->mBitangents[i]));
					}
				}

				if (mesh->mColors != NULL)
				{
					meshData->numColorLayers = mesh->GetNumColorChannels();
					size_t j = 0;
					i = 0;
					meshData->colors.reserve(meshData->numColorLayers);
					while (i < meshData->numColorLayers)
					{
						meshData->colors[i].reserve(meshData->numVertices);
						j = 0;
						while (j < meshData->numVertices)
						{
							meshData->colors[i].push_back(assimpToEigen4F(mesh->mColors[i][j++]));
						}
						i++;
					}
				}

				if (mesh->mTextureCoords != NULL)
				{
					meshData->numUVMaps = mesh->GetNumUVChannels();
					size_t j = 0;
					i = 0;
					meshData->uvs.reserve(meshData->numUVMaps);
					while (i < meshData->numUVMaps)
					{
						meshData->uvs[i].reserve(meshData->numVertices);
						j = 0;
						while (j < meshData->numVertices)
						{
							// FIXME: Assimp returns 3D uv coordinates (UVW). Working for 2D coords right now
							aiVector3D uvCoord = mesh->mTextureCoords[i][j++];
							meshData->uvs[i].push_back(VECTOR2(uvCoord.x, uvCoord.y));
						}
						i++;
					}
				}
			}

			return meshData;
		}
	}
}