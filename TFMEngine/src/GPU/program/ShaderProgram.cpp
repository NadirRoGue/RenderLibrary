#include "GPU/program/ShaderProgram.h"

#include "EngineException.h"

#include <iostream>

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      UberParamMask ShaderProgram::HAS_NORMAL_ATTRIB    = 1;
      UberParamMask ShaderProgram::HAS_TANGENT_ATTRIB   = 2;
      UberParamMask ShaderProgram::HAS_BITANGENT_ATTRIB = 4;
      UberParamMask ShaderProgram::HAS_UV_ATTRIB        = 8;
      UberParamMask ShaderProgram::HAS_COLOR_ATTRIB     = 16;

      UberParamMask ShaderProgram::HAS_DIFFUSE_COLOR     = 32;
      UberParamMask ShaderProgram::HAS_AMBIENT_COLOR     = 64;
      UberParamMask ShaderProgram::HAS_SPECULAR_COLOR    = 128;
      UberParamMask ShaderProgram::HAS_EMISSIVE_COLOR    = 256;
      UberParamMask ShaderProgram::HAS_TRANSPARENT_COLOR = 512;

      UberParamMask ShaderProgram::HAS_OPACITY_VALUE        = 1024;
      UberParamMask ShaderProgram::HAS_SHININESS_VALUE      = 2048;
      UberParamMask ShaderProgram::HAS_SPECULAR_SCALE_VALUE = 4096;
      UberParamMask ShaderProgram::HAS_IOR_VALUE            = 8192;

      UberParamMask ShaderProgram::HAS_NORMALMAP_TEXTURE       = 8192 * 2;
      UberParamMask ShaderProgram::HAS_HEIGHTMAP_TEXTURE       = 8192 * 4;
      UberParamMask ShaderProgram::HAS_DISPLACEMENTMAP_TEXTURE = 8192 * 8;
      UberParamMask ShaderProgram::HAS_REFELCTIONMAP_TEXTURE   = 8192 * 16;
      UberParamMask ShaderProgram::HAS_LIGHTMAP_TEXTURE        = 8192 * 32;
      UberParamMask ShaderProgram::HAS_DIFFUSE_TEXTURE         = 8192 * 64;
      UberParamMask ShaderProgram::HAS_SPECULAR_TEXTURE        = 8192 * 128;
      UberParamMask ShaderProgram::HAS_AMBIENT_TEXTURE         = 8192 * 256;
      UberParamMask ShaderProgram::HAS_SHININESS_TEXTURE       = 8192 * 512;
      UberParamMask ShaderProgram::HAS_EMISSIVE_TEXTURE        = 8192 * 1024;
      UberParamMask ShaderProgram::HAS_OPACITY_TEXTURE         = 8192 * 2048;
      UberParamMask ShaderProgram::HAS_OTHER_TEXTURE           = 8192 * 4096;

      UberParamMask ShaderProgram::FORWARD_SHADING = 8192 * 8192;

      std::unordered_map<UberParamMask, std::string>
          ShaderProgram::maskToDefines = {
            { ShaderProgram::HAS_UV_ATTRIB, "ENABLE_UV" },
            { ShaderProgram::HAS_TANGENT_ATTRIB, "ENABLE_TANGENT" },
            { ShaderProgram::HAS_BITANGENT_ATTRIB, "ENABLE_BITANGENT" },
            { ShaderProgram::HAS_COLOR_ATTRIB, "ENABLE_COLOR" },
            { ShaderProgram::HAS_NORMAL_ATTRIB, "ENABLE_NORMAL" },

            { ShaderProgram::HAS_DIFFUSE_COLOR, "ENABLE_DIFFUSE_COLOR" },
            { ShaderProgram::HAS_AMBIENT_COLOR, "ENABLE_AMBIENT_COLOR" },
            { ShaderProgram::HAS_SPECULAR_COLOR, "ENABLE_SPECULAR_COLOR" },
            { ShaderProgram::HAS_EMISSIVE_COLOR, "ENABLE_EMISSIVE_COLOR" },
            { ShaderProgram::HAS_TRANSPARENT_COLOR,
              "ENABLE_TRANSPARENT_COLOR" },

            { ShaderProgram::HAS_OPACITY_VALUE, "ENABLE_OPACITY_VALUE" },
            { ShaderProgram::HAS_SPECULAR_SCALE_VALUE,
              "ENABLE_SPECULAR_SCALE_VALUE" },
            { ShaderProgram::HAS_SHININESS_VALUE, "ENABLE_SHININESS_VALUE" },
            { ShaderProgram::HAS_IOR_VALUE, "ENABLE_IOR_VALUE" },

            { ShaderProgram::HAS_NORMALMAP_TEXTURE,
              "ENABLE_NORMALMAP_TEXTURE" },
            { ShaderProgram::HAS_HEIGHTMAP_TEXTURE,
              "ENABLE_HEIGHTMAP_TEXTURE" },
            { ShaderProgram::HAS_DISPLACEMENTMAP_TEXTURE,
              "ENABLE_DISPLACEMENTMAP_TEXTURE" },
            { ShaderProgram::HAS_DIFFUSE_TEXTURE, "ENABLE_DIFFUSE_TEXTURE" },
            { ShaderProgram::HAS_SPECULAR_TEXTURE, "ENABLE_SPECULAR_TEXTURE" },
            { ShaderProgram::HAS_AMBIENT_TEXTURE, "ENABLE_AMBIENT_TEXTURE" },
            { ShaderProgram::HAS_EMISSIVE_TEXTURE, "ENABLE_EMISSIVE_TEXTURE" },
            { ShaderProgram::HAS_LIGHTMAP_TEXTURE, "ENABLE_LIGHTMAP_TEXTURE" },
            { ShaderProgram::HAS_SHININESS_TEXTURE,
              "ENABLE_SHININESS_TEXTURE" },
            { ShaderProgram::HAS_OPACITY_TEXTURE, "ENABLE_OPACITY_TEXTURE" },
            { ShaderProgram::HAS_OTHER_TEXTURE, "ENABLE_OTHER_TEXTURE" },
            { ShaderProgram::HAS_REFELCTIONMAP_TEXTURE,
              "ENABLE_REFELECTIONMAP_TEXTURE" },

            { ShaderProgram::FORWARD_SHADING, "FORWARD_SHADING" }

          };

      ShaderProgram::ShaderProgram()
        : Program()
        , vShader(-1)
        , tcShader(-1)
        , teShader(-1)
        , geomShader(-1)
        , fShader(-1)
      {
      }

      ShaderProgram::~ShaderProgram()
      {
      }

      void
      ShaderProgram::getUberShaderDefines(std::vector<std::string> & buffer)
      {
        //std::cout << std::endl;
        for (auto it = maskToDefines.begin(); it != maskToDefines.end(); it++)
        {
          if ((it->first & configMask) == it->first)
          {
            buffer.push_back(it->second);
            //std::cout << it->first << " " << it->second << std::endl;
          }
        }
      }

      void
      ShaderProgram::initialize(std::vector<std::string> & definesBuffer)
      {
        if (vShaderF.empty() || fShaderF.empty())
        {
          throw EngineException(
              "ShaderProgram: A minimun of a vertex shader and a fragment "
              "shader are required to create a ShaderProgram");
        }

        if (tcShaderF.empty() != teShaderF.empty())
        {
          throw EngineException("ShaderProgram: Tesselation stage requires "
                                "both shaders files(control and evaluation)");
        }

        vShader = loadShaderFromFile(GL_VERTEX_SHADER, vShaderF, definesBuffer);
        fShader
            = loadShaderFromFile(GL_FRAGMENT_SHADER, fShaderF, definesBuffer);

        if (!tcShaderF.empty() && !teShaderF.empty())
        {
          tcShader = loadShaderFromFile(GL_TESS_CONTROL_SHADER, tcShaderF,
                                        definesBuffer);
          teShader = loadShaderFromFile(GL_TESS_EVALUATION_SHADER, teShaderF,
                                        definesBuffer);
        }

        if (!geomShaderF.empty())
        {
          geomShader = loadShaderFromFile(GL_GEOMETRY_SHADER, geomShaderF,
                                          definesBuffer);
        }

        attachShader(vShader);
        attachShader(tcShader);
        attachShader(teShader);
        attachShader(geomShader);
        attachShader(fShader);

        link();
      }

      void
      ShaderProgram::destroyShaders()
      {
        detachShader(vShader);
        detachShader(tcShader);
        detachShader(teShader);
        detachShader(geomShader);
        detachShader(fShader);
      }

      void
      ShaderProgram::configureMeshParameters(GPU::Mesh::GPUMesh & gpuMesh)
      {
        configureShaderAttribute("inPos", gpuMesh.vertices);
        configureShaderAttribute("inNormal", gpuMesh.normals);
        configureShaderAttribute("inTangent", gpuMesh.tangents);
        configureShaderAttribute("inBitangent", gpuMesh.bitangents);

        if (gpuMesh.uvs.size() > 0)
        {
          configureShaderAttribute("inUV", gpuMesh.uvs[0]);
        }

        if (gpuMesh.colors.size() > 0)
        {
          configureShaderAttribute("inColor", gpuMesh.colors[0]);
        }
      }

      void
      ShaderProgram::sendMaterialParameters(const Material::Material & material)
      {
        setUniform3FV("diffuseColor", 1, material.diffuseColor().data());
        setUniform3FV("specularColor", 1, material.specularColor().data());
        setUniform3FV("ambientColor", 1, material.ambientColor().data());
        setUniform3FV("emissiveColor", 1, material.emissiveColor().data());
        setUniformF("opacity", material.opacity());
        setUniformF("shininess", material.shininess());
        setUniformF("specularScale", material.specularScale());
        setUniform3FV("transparentColor", 1,
                      material.transparentColor().data());
        setUniformF("indexOfRefraction", material.indexOfRefraction());

        unsigned int texUnit = 0;

        setUniformTexture("diffuseTexture",
                          material.diffuseTexture.getTexture(), texUnit);

        setUniformTexture("specularTexture",
                          material.specularTexture.getTexture(), texUnit);

        setUniformTexture("ambientTexture",
                          material.ambientTexture.getTexture(), texUnit);

        setUniformTexture("emissiveTexture",
                          material.emissiveTexture.getTexture(), texUnit);

        setUniformTexture("normalTexture",
                          material.normalMapTexture.getTexture(), texUnit);

        setUniformTexture("shininessTexture",
                          material.shininessTexture.getTexture(), texUnit);

        setUniformTexture("opacityTexture",
                          material.opacityTexture.getTexture(), texUnit);

        setUniformTexture("lightMapTexture",
                          material.lightMapTexture.getTexture(), texUnit);

        setUniformTexture("heightMapTexture",
                          material.heightMapTexture.getTexture(), texUnit);

        setUniformTexture("displacementTexture",
                          material.displacementTexture.getTexture(), texUnit);

        setUniformTexture("otherTexture", material.otherTexture.getTexture(),
                          texUnit);
      }

      void
      ShaderProgram::sendTransformParameters(const SceneObject & object,
                                             const Camera & camera)
      {
        MATRIX4 modelView
            = camera.viewMatrix * object.transform.worldModelMatrix;
        MATRIX4 modelViewProj = camera.projectionMatrix * modelView;
        MATRIX4 normalMat     = modelView.inverse().transpose();

        setUniformMatrix4("modelView", 1, false, modelView.data());
        setUniformMatrix4("modelViewProj", 1, false, modelViewProj.data());
        setUniformMatrix4("normalMat", 1, false, normalMat.data());
        setUniformMatrix4("model", 1, false,
                          object.transform.worldModelMatrix.data());

        VECTOR3 camPos = -camera.transform.translationV;
        setUniform3FV("viewPos", 1, camPos.data());
      }

      void
      ShaderProgram::onFrameBegin(EngineInstance * instance)
      {
        setUniformBlock("DLBlock",
                        instance->getGPULightManager()
                            .getDirectionalLightBuffer()
                            .getBufferId());
        setUniformBlock(
            "PLBlock",
            instance->getGPULightManager().getPointLightBuffer().getBufferId());
        setUniformBlock(
            "SLBlock",
            instance->getGPULightManager().getSpotLightBuffer().getBufferId());
      }

      void
      ShaderProgram::onRenderObject(DefaultImpl::MeshRenderer & renderable,
                                    const Camera & camera)
      {
      }
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib