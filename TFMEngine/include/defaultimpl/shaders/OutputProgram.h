#ifndef __RENDERLIB_DEFAULTIMPL_OUTPUTPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_OUTPUTPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class OutputProgram : public GPU::Program::PostProcessProgram
    {
    public:
      OutputProgram();
      void
      onRender(GPU::Texture::GPUTexture * previousOutput,
               EngineInstance * instance);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
