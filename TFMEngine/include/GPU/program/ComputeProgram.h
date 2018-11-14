#ifndef __RENDERLIB_GPU_PROGRAM_COMPUTEPROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_COMPUTEPROGRAM_H__

#include "GPU/program/Program.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      class ComputeProgram : public Program
      {
      protected:
        std::string compShaderFile;

      private:
        unsigned int compShader;

      public:
        ComputeProgram();
        ~ComputeProgram();

        void
        initialize(std::vector<std::string> & definesBuffer);

        void
        destroyShaders();

        void
        execute(unsigned int xSize, unsigned int ySize, unsigned int zSize);
      };
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib

#endif