#ifndef __RENDERLIB_CPU_MESHLOADRESULT_H__
#define __RENDERLIB_CPU_MESHLOADRESULT_H__

#include <vector>

#include "CPU/io/FileLoadResult.h"
#include "Defines.h"

#include "CPU/texture/TextureStackBlendOperation.h"

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

        unsigned int materialIndex;
        std::string srcFile;

        std::vector<IVECTOR3> loadedFaces;
        std::vector<VECTOR3> loadedVertices;
        std::vector<VECTOR3> loadedNormals;
        std::vector<VECTOR3> loadedTangents;
        std::vector<VECTOR3> loadedBitangents;
        std::vector<std::vector<VECTOR2>> loadedUvs;
        std::vector<std::vector<VECTOR4>> loadedColors;
      } MeshLoadedData;

      typedef struct MaterialTextureInfo
      {
      public:
        std::string filePath;
        Texture::TextureStackBlendOperation blendOperation;
        float blendStrength;

        MaterialTextureInfo()
          : filePath("")
          , blendOperation(Texture::TextureStackBlendOperation::OP_ADD)
          , blendStrength(1.0f)
        {
        }

      } MaterialTextureInfo;

      typedef struct MaterialLoadedData
      {
        unsigned int index;
        IO::LoadedParameter<std::string> name;
        IO::LoadedParameter<VECTOR3> diffuseColor;
        IO::LoadedParameter<VECTOR3> specularColor;
        IO::LoadedParameter<VECTOR3> ambientColor;
        IO::LoadedParameter<VECTOR3> emissiveColor;
        IO::LoadedParameter<VECTOR3> transparentColor; // wtf
        IO::LoadedParameter<FLOAT> opacity;
        IO::LoadedParameter<FLOAT> shininess;
        IO::LoadedParameter<FLOAT> specularScale; // A.K.A shininessStrength
        IO::LoadedParameter<FLOAT> indexOfRefraction;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> diffuseTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> specularTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> shininessTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> ambientTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> emissiveTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> normalMapTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> opacityTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> heightMapTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>>
            displacementTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> lightMapTextures;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> reflectionTexture;
        std::vector<IO::LoadedParameter<MaterialTextureInfo>> otherTextures;
        bool wireFrameRender;
        bool twoSidedRender;

        MaterialLoadedData() : wireFrameRender(false), twoSidedRender(false)
        {
        }

      } MaterialLoadedData;

      class MeshLoadResult : public IO::AbstractLoadResult
      {
      public:
        std::vector<MeshLoadedData> loadedData;
        std::vector<MaterialLoadedData> loadedMaterials;

      public:
        size_t
        getResultSizeBytes();
      };
    } // namespace Mesh
  } // namespace CPU
} // namespace RenderLib

#endif
