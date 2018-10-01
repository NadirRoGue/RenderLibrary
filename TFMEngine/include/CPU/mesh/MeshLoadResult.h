#ifndef __RENDERLIB_CPU_MESHLOADRESULT_H__
#define __RENDERLIB_CPU_MESHLOADRESULT_H__

#include <vector>

#include "Defines.h"
#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			typedef struct MeshLoadedData
			{
				size_t numFaces;
				size_t numVertices;

				size_t numUVMaps;
				size_t numColorLayers;

				std::vector<IVECTOR3> loadedFaces;
				std::vector<VECTOR3> loadedVertices;
				std::vector<VECTOR3> loadedNormals;
				std::vector<VECTOR3> loadedTangents;
				std::vector<VECTOR3> loadedBitangents;
				std::vector<std::vector<VECTOR2>> loadedUvs;
				std::vector<std::vector<VECTOR4>> loadedColors;
			} MeshLoadedData;

			typedef struct MaterialLoadedData
			{
				unsigned int index;
				IO::LoadedParameter<std::string> name;
				IO::LoadedParameter<VECTOR3> diffuseColor;
				IO::LoadedParameter<VECTOR3> specularColor;
				IO::LoadedParameter<VECTOR3> ambientColor;
				IO::LoadedParameter<VECTOR3> emissiveColor;
				IO::LoadedParameter<VECTOR3> transparentColor; //wtf
				IO::LoadedParameter<FLOAT> opacity;
				IO::LoadedParameter<FLOAT> shininess;
				IO::LoadedParameter<FLOAT> specularScale; // A.K.A shininessStrength
				IO::LoadedParameter<FLOAT> indexOfRefraction;
				std::vector<IO::LoadedParameter<std::string>> diffuseTextures;
				std::vector<IO::LoadedParameter<std::string>> specularTextures;
				std::vector<IO::LoadedParameter<std::string>> shininessTextures;
				std::vector<IO::LoadedParameter<std::string>> ambientTextures;
				std::vector<IO::LoadedParameter<std::string>> emissiveTextures;
				std::vector<IO::LoadedParameter<std::string>> normalMapTextures;
				std::vector<IO::LoadedParameter<std::string>> opacityTextures;
				std::vector<IO::LoadedParameter<std::string>> heightMapTextures;
				std::vector<IO::LoadedParameter<std::string>> displacementTextures;
				std::vector<IO::LoadedParameter<std::string>> otherTextures;
				bool wireFrameRender;
				bool twoSidedRender;

				MaterialLoadedData()
					: wireFrameRender(false)
					, twoSidedRender(false)
				{
				}

			} MaterialLoadedData;

			enum class MaterialTextureType : int
			{
				TEXTURE_DIFFUSE,
				TEXTURE_SPECULAR,
				TEXTURE_SHININESS,
				TEXTURE_AMBIENT,
				TEXTURE_EMISSIVE,
				TEXTURE_NORMALMAP,
				TEXTURE_HEIGHTMAP,
				TEXTURE_DISPLACEMENT,
			};

			class MeshLoadResult : public IO::AbstractLoadResult
			{
			public:
				std::vector<MeshLoadedData> loadedData;
				std::vector<MaterialLoadedData> loadedMaterials;
			public:
				size_t getResultSizeBytes();
			};
		}
	}
}

#endif