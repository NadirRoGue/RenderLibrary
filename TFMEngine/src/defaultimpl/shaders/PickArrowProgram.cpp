#include "defaultimpl/shaders/PickArrowProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    PickArrowProgram::PickArrowProgram() : GPU::Program::ShaderProgram()
    {
      vShaderF = "shaders/default/pickarrow.vert";
      fShaderF = "shaders/default/pickarrow.frag";
    }

    PickArrowProgram::~PickArrowProgram()
    {
    }
  } // namespace DefaultImpl
} // namespace RenderLib