#ifndef __RENDERLIB_GPU_PROGRAM_POSTPROCESSPROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_POSTPROCESSPROGRAM_H__

#include "GPU/program/Program.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      class PostProcessProgram : public Program
      {
      private:
        std::string vShaderFile;

        unsigned int vShader;
        unsigned int fShader;

      protected:
        std::string fShaderFile;

      public:
        PostProcessProgram();
        ~PostProcessProgram();

        void
        initialize(std::vector<std::string> & definesBuffer);

        void
        destroyShaders();

        void
        configureRenderQuad(GPU::Mesh::GPUMesh * quad);

        virtual void
        initializeShader(EngineInstance * instance);
        virtual void
        onRender(GPU::Texture::GPUTexture * previousOutput,
                 EngineInstance * instance);
      };
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib

#endif