#ifndef __RENDERLIB_DEFAULTIMPL_HDRPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_HDRPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class HDRProgram : public GPU::Program::PostProcessProgram
    {
    public:
      HDRProgram();
      void
      onRender(GPU::Texture::GPUTexture * previousOutput,
               EngineInstance * instance);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
