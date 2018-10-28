#include "defaultimpl/shaders/PickingProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		PickingProgram::PickingProgram()
			: GPU::Program::ShaderProgram()
		{
			vShaderF = "shaders/default/picking.vert";
			fShaderF = "shaders/default/picking.frag";
		}

		PickingProgram::~PickingProgram()
		{

		}
	}
}