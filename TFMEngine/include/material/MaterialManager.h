#ifndef __RENDERLIB_CPU_MATERIALMANAGER_H__
#define __RENDERLIB_CPU_MATERIALMANAGER_H__

#include <unordered_map>
#include <memory>
#include <mutex>

#include "Material.h"
#include "CPU/mesh/MeshLoadResult.h"

namespace RenderLib
{
	namespace Material
	{
		typedef std::unordered_map<unsigned int, std::unique_ptr<Material>> SceneMaterialTable;

		class MaterialManager
		{
		private:
			static MaterialManager INSTANCE;

			static Material dummyMaterial;
		public:
			static MaterialManager & getInstance();
		private:
			std::unordered_map<std::string, SceneMaterialTable> materials;
			std::mutex mtx;
		private:
			MaterialManager();
		public:
			~MaterialManager();

			void parseMaterials(const std::string & fileName, std::vector<CPU::Mesh::MaterialLoadedData> & data);

			Material * getMaterial(const std::string & fileName, const unsigned int & index);
			Material * getMaterial(const std::string & fileName, const std::string & materialName);

			std::vector<Material *> getAllMaterialsOfFile(const std::string & fileName);
		private:
			void parseMaterial(const std::string & fileName, CPU::Mesh::MaterialLoadedData & materialData);
			void parseMaterialTexture(const std::string & textureName,
				const VECTOR3 & baseColor, 
				std::vector<CPU::IO::LoadedParameter<CPU::Mesh::MaterialTextureInfo>> & info, 
				MaterialTexture & dstObj);
		};
	}
}

#endif