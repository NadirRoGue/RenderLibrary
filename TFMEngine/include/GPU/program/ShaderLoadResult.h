#ifndef __RENDERLIB_GPU_PROGRAM_SHADERLOADRESULT_H__
#define __RENDERLIB_GPU_PROGRAM_SHADERLOADRESULT_H__

#include <vector>

#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      class ShaderLoadResult : public CPU::IO::AbstractLoadResult
      {
      public:
        std::string header;
        std::string body;
        std::vector<char> text;

      public:
        size_t
        getResultSizeBytes();
      };
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib

#endif