#include "defaultimpl/fileloaders/AssimpFileLoader.h"

#include "Defines.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "CPU/mesh/MeshManager.h"

#include "EngineException.h"

#include "util/StringUtils.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		// ===========================================================================
		// Assimp to Eigen data structures utility functions
		template<typename T>
		inline T dummyFunc(const T & val)
		{
			return val;
		}

		inline std::string assimpToString(const aiString & source)
		{
			return std::string(source.C_Str());
		}

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

		template<class SRC, class DST, typename CONVFUNC>
		inline void gatherMaterialValue(aiMaterial * mat, const char * pkey, unsigned int type, unsigned int idx, CPU::IO::LoadedParameter<DST> &dst, CONVFUNC func)
		{
			SRC storage;
			if (mat->Get(pkey, type, idx, storage) == AI_SUCCESS)
			{
				dst.set(func(storage));
			}
		}

		inline void gatherMaterialTexturesOfType(aiMaterial * mat, const aiTextureType & textureType, std::vector<CPU::IO::LoadedParameter<std::string>> & dst, const std::string & rootPath)
		{
			unsigned int numTexturesInStack = mat->GetTextureCount(textureType);
			dst.resize(numTexturesInStack);
			for (unsigned int i = 0; i < numTexturesInStack; i++)
			{
				gatherMaterialValue<aiString, std::string>(mat, AI_MATKEY_TEXTURE(textureType, i), dst[i], assimpToString);
				std::string relativePath = dst[i].get();
				dst[i].set(rootPath + "/" + relativePath);
			}
		}

		//void gatherMaterialTexture(aiMaterial * mat)

		// ===========================================================================

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

		CPU::IO::AbstractLoadResultPtr AssimpFileLoader::loadFile(const std::string & fileName, unsigned int options)
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

			std::unique_ptr<CPU::Mesh::MeshLoadResult> result = std::make_unique<CPU::Mesh::MeshLoadResult>();

			size_t lastIndexSlash = fileName.find_last_of("/\\");
			std::string rootPath = lastIndexSlash == std::string::npos? "." : fileName.substr(0, lastIndexSlash);
			processSceneMaterials(scene, result.get(), rootPath);
			processSceneMeshes(scene, result.get());

			return result;
		}

		void AssimpFileLoader::processSceneMaterials(const aiScene * scene, CPU::Mesh::MeshLoadResult * dst, const std::string & rootPath)
		{
			if (!scene->HasMaterials())
			{
				return;
			}
		 
			for (unsigned int i = 0; i < scene->mNumMaterials; i++)
			{
				aiMaterial * material = scene->mMaterials[i];
				CPU::Mesh::MaterialLoadedData matData;
				matData.index = i;
				processFileMaterial(material, matData, rootPath);
				dst->loadedMaterials.push_back(matData);
			}
		}

		void AssimpFileLoader::processFileMaterial(aiMaterial * material, CPU::Mesh::MaterialLoadedData & matData, const std::string & rootPath)
		{
			gatherMaterialValue<aiString, std::string>(material, AI_MATKEY_NAME, matData.name, assimpToString);
			gatherMaterialValue<aiVector3D, VECTOR3>(material, AI_MATKEY_COLOR_DIFFUSE, matData.diffuseColor, assimpToEigen3F);
			gatherMaterialValue<aiVector3D, VECTOR3>(material, AI_MATKEY_COLOR_SPECULAR, matData.specularColor, assimpToEigen3F);
			gatherMaterialValue<aiVector3D, VECTOR3>(material, AI_MATKEY_COLOR_AMBIENT, matData.ambientColor, assimpToEigen3F);
			gatherMaterialValue<aiVector3D, VECTOR3>(material, AI_MATKEY_COLOR_EMISSIVE, matData.emissiveColor, assimpToEigen3F);
			gatherMaterialValue<aiVector3D, VECTOR3>(material, AI_MATKEY_COLOR_TRANSPARENT, matData.transparentColor, assimpToEigen3F);
			gatherMaterialValue<FLOAT, FLOAT>(material, AI_MATKEY_OPACITY, matData.opacity, dummyFunc<FLOAT>);
			gatherMaterialValue<FLOAT, FLOAT>(material, AI_MATKEY_SHININESS, matData.shininess, dummyFunc<FLOAT>);
			gatherMaterialValue<FLOAT, FLOAT>(material, AI_MATKEY_SHININESS_STRENGTH, matData.specularScale, dummyFunc<FLOAT>);
			gatherMaterialValue<FLOAT, FLOAT>(material, AI_MATKEY_REFRACTI, matData.indexOfRefraction, dummyFunc<FLOAT>);
			gatherMaterialTexturesOfType(material, aiTextureType_DIFFUSE, matData.diffuseTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_SPECULAR, matData.specularTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_SHININESS, matData.shininessTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_EMISSIVE, matData.emissiveTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_AMBIENT, matData.ambientTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_OPACITY, matData.opacityTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_NORMALS, matData.normalMapTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_HEIGHT, matData.heightMapTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_DISPLACEMENT, matData.displacementTextures, rootPath);
			gatherMaterialTexturesOfType(material, aiTextureType_UNKNOWN, matData.otherTextures, rootPath);
			// NOT DONE: aiTextureType_LIGHTMAP, aiTextureType_REFLECTION

			bool renderMode = false;
			if (material->Get(AI_MATKEY_ENABLE_WIREFRAME, renderMode) == AI_SUCCESS)
			{
				matData.wireFrameRender = renderMode;
			}

			bool twoSided = false;
			if (material->Get(AI_MATKEY_TWOSIDED, twoSided) == AI_SUCCESS)
			{
				matData.twoSidedRender = twoSided;
			}
		}

		void AssimpFileLoader::processSceneMeshes(const aiScene * scene, CPU::Mesh::MeshLoadResult * dst)
		{
			if (!scene->HasMeshes())
			{
				return;
			}

			size_t numMeshes = scene->mNumMeshes;
			size_t i = 0;
			while (i < numMeshes)
			{
				aiMesh * meshToProcess = scene->mMeshes[i];

				CPU::Mesh::MeshLoadedData meshDst;
				processFileMesh(meshToProcess, meshDst);

				dst->loadedData.push_back(meshDst);
				i++;
			}
		}

		void AssimpFileLoader::processFileMesh(aiMesh * mesh, CPU::Mesh::MeshLoadedData & dst)
		{
			std::unique_ptr<CPU::Mesh::MeshLoadResult> meshDataPtr = std::make_unique<CPU::Mesh::MeshLoadResult>();
			CPU::Mesh::MeshLoadResult * meshData = meshDataPtr.get();

			size_t i = 0;

			dst.numFaces = mesh->mNumFaces;
			dst.loadedFaces.reserve(dst.numFaces);
			while (i < dst.numFaces)
			{
				aiFace & face = mesh->mFaces[i++];
				dst.loadedFaces.push_back(assimpToEigen3I(face));
			}

			i = 0;

			if (mesh->mNumVertices > 0)
			{
				dst.numVertices = mesh->mNumVertices;
				dst.loadedVertices.reserve(dst.numVertices);
				while (i < dst.numVertices)
				{
					dst.loadedVertices.push_back(assimpToEigen3F(mesh->mVertices[i++]));
				}

				if (mesh->mNormals != NULL)
				{
					i = 0;
					dst.loadedNormals.reserve(dst.numVertices);
					while (i < dst.numVertices)
					{
						dst.loadedNormals.push_back(assimpToEigen3F(mesh->mNormals[i++]));
					}
				}

				if (mesh->mTangents != NULL)
				{
					dst.loadedTangents.reserve(dst.numVertices);
					i = 0;
					while (i < dst.numVertices)
					{
						dst.loadedTangents.push_back(assimpToEigen3F(mesh->mTangents[i++]));
					}
				}

				if (mesh->mBitangents != NULL)
				{
					dst.loadedBitangents.reserve(dst.numVertices);
					i = 0;
					while (i < dst.numVertices)
					{
						dst.loadedBitangents.push_back(assimpToEigen3F(mesh->mBitangents[i++]));
					}
				}

				if (mesh->mColors != NULL)
				{
					dst.numColorLayers = mesh->GetNumColorChannels();
					size_t j = 0;
					i = 0;
					dst.loadedColors.resize(dst.numColorLayers);
					while (i < dst.numColorLayers)
					{
						dst.loadedColors[i].reserve(dst.numVertices);
						j = 0;
						while (j < dst.numVertices)
						{
							dst.loadedColors[i].push_back(assimpToEigen4F(mesh->mColors[i][j++]));
						}
						i++;
					}
				}

				if (mesh->mTextureCoords != NULL)
				{
					dst.numUVMaps = mesh->GetNumUVChannels();
					size_t j = 0;
					i = 0;
					dst.loadedUvs.resize(dst.numUVMaps);
					while (i < dst.numUVMaps)
					{
						dst.loadedUvs[i].reserve(dst.numVertices);
						j = 0;
						while (j < dst.numVertices)
						{
							// FIXME: Assimp returns 3D uv coordinates (UVW). Working for 2D coords right now
							aiVector3D uvCoord = mesh->mTextureCoords[i][j++];
							dst.loadedUvs[i].push_back(VECTOR2(uvCoord.x, uvCoord.y));
						}
						i++;
					}
				}
			}
		}
	}
}