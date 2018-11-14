#include "defaultimpl/shaders/PickingProgram.h"

#include "defaultimpl/components/PickArrowMovement.h"

#include <iostream>

namespace RenderLib
{
  namespace DefaultImpl
  {
    PickingProgram::PickingProgram() : GPU::Program::ShaderProgram()
    {
      vShaderF = "shaders/default/picking.vert";
      fShaderF = "shaders/default/picking.frag";
    }

    PickingProgram::~PickingProgram()
    {
    }
  } // namespace DefaultImpl
} // namespace RenderLib