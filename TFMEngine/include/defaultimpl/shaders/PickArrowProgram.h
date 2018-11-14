#ifndef __RENDERLIB_DEFAULTIMPL_PICKARROWPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_PICKARROWPROGRAM_H__

#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class PickArrowProgram : public GPU::Program::ShaderProgram
    {
    public:
      PickArrowProgram();
      ~PickArrowProgram();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif