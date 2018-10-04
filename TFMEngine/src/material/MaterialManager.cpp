#include "material/MaterialManager.h"

#include "CPU/texture/TextureManager.h"

#include <iostream>

namespace RenderLib
{
	namespace Material
	{
		MaterialManager MaterialManager::INSTANCE;

		Material MaterialManager::dummyMaterial;

		MaterialManager & MaterialManager::getInstance()
		{
			return INSTANCE;
		}

		MaterialManager::MaterialManager()
		{
		}

		MaterialManager::~MaterialManager()
		{
		}

		void MaterialManager::parseMaterials(const std::string & fileName, std::vector<CPU::Mesh::MaterialLoadedData> & data)
		{
			std::unique_lock<std::mutex> lock(mtx);
			for (auto & matData : data)
			{
				parseMaterial(fileName, matData);
			}
			lock.unlock();
		}

		void MaterialManager::parseMaterial(const std::string & fileName, CPU::Mesh::MaterialLoadedData & data)
		{
			std::unique_ptr<Material> newMaterial = std::make_unique<Material>();
			Material * ptr = newMaterial.get();

			ptr->index = data.index;
			ptr->name.setData(data.name.get(), data.name.paramExists());
			ptr->ambientColor.setData(data.ambientColor.get(), data.ambientColor.paramExists());
			ptr->diffuseColor.setData(data.diffuseColor.get(), data.diffuseColor.paramExists());
			ptr->specularColor.setData(data.specularColor.get(), data.specularColor.paramExists());
			ptr->emissiveColor.setData(data.emissiveColor.get(), data.emissiveColor.paramExists());
			ptr->transparentColor.setData(data.transparentColor.get(), data.transparentColor.paramExists());
			ptr->opacity.setData(data.opacity.get(), data.opacity.paramExists());
			ptr->shininess.setData(data.shininess.get(), data.shininess.paramExists());
			ptr->specularScale.setData(data.specularScale.get(), data.specularScale.paramExists());
			ptr->indexOfRefraction.setData(data.indexOfRefraction.get(), data.indexOfRefraction.paramExists());
			
			std::string baseName = fileName + "_" + std::to_string(data.index) + "_";
			parseMaterialTexture(baseName+"_diffuse", data.diffuseColor.get(), data.diffuseTextures, ptr->diffuseTexture);
			parseMaterialTexture(baseName+"_specular", data.specularColor.get(), data.specularTextures, ptr->specularTexture);
			parseMaterialTexture(baseName+"_ambient", data.ambientColor.get(), data.ambientTextures, ptr->ambientTexture);
			parseMaterialTexture(baseName+"_displacement", VECTOR3(0,0,0), data.displacementTextures, ptr->displacementTexture);
			parseMaterialTexture(baseName+"_emissive", data.emissiveColor.get(), data.emissiveTextures, ptr->emissiveTexture);
			parseMaterialTexture(baseName+"_heightmap", VECTOR3(0,0,0), data.heightMapTextures, ptr->heightMapTexture);
			parseMaterialTexture(baseName+"_normalmap", VECTOR3(0,0,0), data.normalMapTextures, ptr->normalMapTexture);
			parseMaterialTexture(baseName+"_transparent", data.transparentColor.get(), data.opacityTextures, ptr->opacityTexture);
			parseMaterialTexture(baseName+"_other", VECTOR3(0,0,0), data.otherTextures, ptr->otherTexture);
			parseMaterialTexture(baseName+"_shininess", VECTOR3(0,0,0), data.shininessTextures, ptr->shininessTexture);
			
			materials[fileName][ptr->index] = std::move(newMaterial);
		}

		void MaterialManager::parseMaterialTexture(const std::string & textureName,
			const VECTOR3 & baseColor,
			std::vector<CPU::IO::LoadedParameter<CPU::Mesh::MaterialTextureInfo>> & info,
			MaterialTexture & dstObj)
		{
			if (info.size() > 0)
			{
				std::string usedName = "";
				if (info.size() == 1)
				{
					usedName = info[0].get().filePath;
					CPU::Texture::TextureManager::getInstance().loadTexture(usedName, CPU::Texture::TextureType::TEXTURE2D);
				}
				else
				{
					usedName = textureName;
					CPU::Texture::TextureManager::getInstance().loadAndProcessTextureStack(textureName, baseColor, info);
				}

				dstObj.fileName = usedName;
			}
		}

		Material * MaterialManager::getMaterial(const std::string & fileName, const unsigned int & index)
		{
			auto it = materials.find(fileName);
			if (it != materials.end())
			{
				auto it2 = it->second.find(index);
				if (it2 != it->second.end())
				{
					return it2->second.get();
				}
			}

			return &dummyMaterial;
		}

		Material * MaterialManager::getMaterial(const std::string & fileName, const std::string & materialName)
		{
			auto it = materials.find(fileName);
			if (it != materials.end())
			{
				bool found = false;
				Material * result = NULL;
				for (auto innerIt = it->second.begin(); innerIt != it->second.end() && !found; innerIt++)
				{
					Material * mat = innerIt->second.get();
					if (mat->name.get() == fileName)
					{
						result = mat;
						found = true;
					}
				}

				return result;
			}

			return &dummyMaterial;
		}

		std::vector<Material*> MaterialManager::getAllMaterialsOfFile(const std::string & fileName)
		{
			std::vector<Material *> mats;

			auto it = materials.find(fileName);
			if (it != materials.end())
			{
				for (auto inner = it->second.begin(); inner != it->second.end(); it++)
				{
					mats.push_back(inner->second.get());
				}
			}

			return mats;
		}
	}
}