#ifndef __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__

#include "GPU/program/ShaderProgram.h"

#include "GPU/program/UberMaskFactory.h"

#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class StandardProgram : public GPU::Program::ShaderProgram
    {
    public:
      StandardProgram();
      ~StandardProgram();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif