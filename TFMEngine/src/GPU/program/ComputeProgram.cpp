#include "GPU/program/ComputeProgram.h"

#include "EngineException.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      ComputeProgram::ComputeProgram() : Program(), compShader(-1)
      {
      }

      ComputeProgram::~ComputeProgram()
      {
      }

      void
      ComputeProgram::initialize(std::vector<std::string> & definesBuffer)
      {
        if (compShaderFile.empty())
        {
          throw EngineException("ComputeShader: No compute shader file setted");
        }

        compShader = loadShaderFromFile(GL_COMPUTE_SHADER, compShaderFile,
                                        definesBuffer);
        attachShader(compShader);
        link();
      }

      void
      ComputeProgram::destroyShaders()
      {
        detachShader(compShader);
      }

      void
      ComputeProgram::execute(unsigned int xSize, unsigned int ySize,
                              unsigned int zSize)
      {
      }
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib