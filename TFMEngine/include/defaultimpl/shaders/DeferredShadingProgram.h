#ifndef __RENDERLIB_DEFAULTIMPL_DEFERREDSHADINGPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_DEFERREDSHADINGPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class DeferredShadingProgram : public GPU::Program::PostProcessProgram
    {
    public:
      DeferredShadingProgram();
      void
      onRender(EngineInstance * instance);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
