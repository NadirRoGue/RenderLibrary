#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAM_H__

#include <GL/glew.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "Defines.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      typedef unsigned long long UberParamMask;

      typedef struct ShaderInput
      {
        unsigned int id;
        GLint size;
        GLenum type;
      } ShaderInput;

      class Program
      {
      protected:
        UberParamMask configMask;

      protected:
        std::unordered_map<std::string, ShaderInput> shaderUniforms;
        std::unordered_map<std::string, ShaderInput> shaderAttributes;
        std::unordered_map<std::string, ShaderInput> shaderBlocks;

      public:
        unsigned int programId;

      public:
        Program();
        ~Program();

        void
        bind();
        void
        unBind();

        const UberParamMask &
        getConfigMask();

        void
        init(const UberParamMask & mask);

        void
        destroy();

        void
        setUniformI(const std::string & name, const int & val);
        void
        setUniformI2(const std::string & name, const int & v1, const int & v2);
        void
        setUniformI3(const std::string & name, const int & v1, const int & v2,
                     const int & v3);
        void
        setUniformI4(const std::string & name, const int & v1, const int & v2,
                     const int & v3, const int & v4);
        void
        setUniformI2V(const std::string & name, const unsigned int & count,
                      const int * val);
        void
        setUniformI3V(const std::string & name, const unsigned int & count,
                      const int * val);
        void
        setUniformI4V(const std::string & name, const unsigned int & count,
                      const int * val);
        void
        setUniformF(const std::string & name, const FLOAT & val);
        void
        setUniform2F(const std::string & name, const FLOAT & v1,
                     const FLOAT & v2);
        void
        setUniform3F(const std::string & name, const FLOAT & v1,
                     const FLOAT & v2, const FLOAT & v3);
        void
        setUniform4F(const std::string & name, const FLOAT & v1,
                     const FLOAT & v2, const FLOAT & v3, const FLOAT & v4);
        void
        setUniform2FV(const std::string & name, const unsigned int & count,
                      const FLOAT * val);
        void
        setUniform3FV(const std::string & name, const unsigned int & count,
                      const FLOAT * val);
        void
        setUniform4FV(const std::string & name, const unsigned int & count,
                      const FLOAT * val);
        void
        setUniformUI(const std::string & name, const unsigned int & val);
        void
        setUniformUI2(const std::string & name, const unsigned int & v1,
                      const unsigned int & v2);
        void
        setUniformUI3(const std::string & name, const unsigned int & v1,
                      const unsigned int & v2, const unsigned int & v3);
        void
        setUniformUI4(const std::string & name, const unsigned int & v1,
                      const unsigned int & v2, const unsigned int & v3,
                      const unsigned int & v4);
        void
        setUniformUI2V(const std::string & name, const unsigned int & count,
                       const unsigned int * val);
        void
        setUniformUI3V(const std::string & name, const unsigned int & count,
                       const unsigned int * val);
        void
        setUniformUI4V(const std::string & name, const unsigned int & count,
                       const unsigned int * val);
        void
        setUniformMatrix2(const std::string & name, const unsigned int & count,
                          const bool & transpose, const FLOAT * val);
        void
        setUniformMatrix3(const std::string & name, const unsigned int & count,
                          const bool & transpose, const FLOAT * val);
        void
        setUniformMatrix4(const std::string & name, const unsigned int & count,
                          const bool & transpose, const FLOAT * val);
        void
        setUniformTexture(const std::string & name,
                          Texture::GPUTexture * texture,
                          unsigned int & textureUnit);
        void
        setUniformBlock(const std::string & name, const unsigned int & ubo);

      protected:
        virtual void
        getUberShaderDefines(std::vector<std::string> & definesBuffer);
        virtual unsigned int
        loadShaderFromFile(GLenum shaderType, const std::string & filePath,
                           std::vector<std::string> & configStrings);

        virtual void
        initialize(std::vector<std::string> & definesBuffer)
            = 0;
        virtual void
        destroyShaders();

        void
        attachShader(unsigned int shaderId);
        void
        detachShader(unsigned int shaderId, bool deleteShader = true);
        void
        link();

        bool
        hasMask(const UberParamMask & prop);

        void
        configureShaderAttribute(const std::string & shaderAttribute,
                                 Mesh::GPUAttribute & meshAttribute);

      private:
        void
        gatherInputs();
      };
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib

#endif