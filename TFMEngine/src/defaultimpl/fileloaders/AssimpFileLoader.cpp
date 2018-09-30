#include "defaultimpl/fileloaders/AssimpFileLoader.h"

#include "Defines.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "CPU/mesh/MeshManager.h"

#include "EngineException.h"

#include "util/StringUtils.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		inline VECTOR2 assimpToEigen2F(const aiVector2D & source)
		{
			return (VECTOR2(source.x, source.y));
		}

		inline VECTOR3 assimpToEigen3F(const aiVector3D & source)
		{
			return (VECTOR3(source.x, source.y, source.z));
		}

		inline VECTOR4 assimpToEigen4F(const aiColor4D & source)
		{
			return (VECTOR4(source.r, source.g, source.b, source.a));
		}

		inline IVECTOR3 assimpToEigen3I(const aiFace & face)
		{
			return (IVECTOR3(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
		}

		AssimpFileLoader::AssimpFileLoader()
		{
			Assimp::Importer temp;
			std::string extensions;
			temp.GetExtensionList(extensions);

			std::vector<std::string> splitExtensions = StringUtils::split(extensions, ";");
			for (auto & rawExt : splitExtensions)
			{
				size_t dotPos = rawExt.find_first_of(".");
				if (dotPos != std::string::npos)
				{
					std::string ext = rawExt.substr(dotPos + 1, rawExt.length() - 1 - dotPos);
					registerFileExtension(ext);
				}
			}
		}

		std::vector<CPU::IO::AbstractLoadResultPtr> AssimpFileLoader::loadFile(const std::string & fileName, unsigned int options)
		{
			Assimp::Importer importer;

			unsigned int assimpFlags = 0;

			assimpFlags |= aiPostProcessSteps::aiProcess_JoinIdenticalVertices;
			assimpFlags |= (options & CPU::Mesh::MeshManager::OPTION_COMPUTE_NORMALS_IF_ABSENT) ? aiProcess_GenSmoothNormals : 0;
			assimpFlags |= (options & CPU::Mesh::MeshManager::OPTION_COMPUTE_TANGENTS_IF_ABSENT
				|| options & CPU::Mesh::MeshManager::OPTION_COMPUTE_BITANGENTS_IF_ABSENT) ? aiProcess_CalcTangentSpace : 0;
			assimpFlags |= aiProcess_Triangulate;

			const aiScene * scene = importer.ReadFile(fileName, assimpFlags);

			if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			{
				std::string importerError(importer.GetErrorString());
				throw EngineException("AssimpFileLoader: Error importing mesh " + fileName + ": " + importerError);
			}

			std::vector<CPU::IO::AbstractLoadResultPtr> result;
			size_t numMeshes = scene->mNumMeshes;
			size_t i = 0;
			while (i < numMeshes)
			{
				aiMesh * meshToProcess = scene->mMeshes[i];

				std::unique_ptr<CPU::Mesh::MeshLoadResult> engineMesh = processFileMesh(meshToProcess, options);

				result.push_back(std::move(engineMesh));
				i++;
			}

			return result;
		}

		std::unique_ptr<CPU::Mesh::MeshLoadResult> AssimpFileLoader::processFileMesh(aiMesh * mesh, unsigned int options)
		{
			std::unique_ptr<CPU::Mesh::MeshLoadResult> meshDataPtr = std::make_unique<CPU::Mesh::MeshLoadResult>();
			CPU::Mesh::MeshLoadResult * meshData = meshDataPtr.get();

			size_t i = 0;

			meshData->numFaces = mesh->mNumFaces;
			meshData->loadedFaces.reserve(meshData->numFaces);
			while (i < meshData->numFaces)
			{
				aiFace & face = mesh->mFaces[i++];
				meshData->loadedFaces.push_back(assimpToEigen3I(face));
			}

			i = 0;

			if (mesh->mNumVertices > 0)
			{
				meshData->numVertices = mesh->mNumVertices;
				meshData->loadedVertices.reserve(meshData->numVertices);
				while (i < meshData->numVertices)
				{
					meshData->loadedVertices.push_back(assimpToEigen3F(mesh->mVertices[i++]));
				}

				if (mesh->mNormals != NULL)
				{
					i = 0;
					meshData->loadedNormals.reserve(meshData->numVertices);
					while (i < meshData->numVertices)
					{
						meshData->loadedNormals.push_back(assimpToEigen3F(mesh->mNormals[i++]));
					}
				}

				if (mesh->mTangents != NULL)
				{
					meshData->loadedTangents.reserve(meshData->numVertices);
					i = 0;
					while (i < meshData->numVertices)
					{
						meshData->loadedTangents.push_back(assimpToEigen3F(mesh->mTangents[i++]));
					}
				}

				if (mesh->mBitangents != NULL)
				{
					meshData->loadedBitangents.reserve(meshData->numVertices);
					i = 0;
					while (i < meshData->numVertices)
					{
						meshData->loadedBitangents.push_back(assimpToEigen3F(mesh->mBitangents[i++]));
					}
				}

				if (mesh->mColors != NULL)
				{
					meshData->numColorLayers = mesh->GetNumColorChannels();
					size_t j = 0;
					i = 0;
					meshData->loadedColors.resize(meshData->numColorLayers);
					while (i < meshData->numColorLayers)
					{
						meshData->loadedColors[i].reserve(meshData->numVertices);
						j = 0;
						while (j < meshData->numVertices)
						{
							meshData->loadedColors[i].push_back(assimpToEigen4F(mesh->mColors[i][j++]));
						}
						i++;
					}
				}

				if (mesh->mTextureCoords != NULL)
				{
					meshData->numUVMaps = mesh->GetNumUVChannels();
					size_t j = 0;
					i = 0;
					meshData->loadedUvs.resize(meshData->numUVMaps);
					while (i < meshData->numUVMaps)
					{
						meshData->loadedUvs[i].reserve(meshData->numVertices);
						j = 0;
						while (j < meshData->numVertices)
						{
							// FIXME: Assimp returns 3D uv coordinates (UVW). Working for 2D coords right now
							aiVector3D uvCoord = mesh->mTextureCoords[i][j++];
							meshData->loadedUvs[i].push_back(VECTOR2(uvCoord.x, uvCoord.y));
						}
						i++;
					}
				}
			}

			return meshDataPtr;
		}
	}
}