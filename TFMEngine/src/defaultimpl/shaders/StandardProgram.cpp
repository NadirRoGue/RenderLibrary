#include "defaultimpl/shaders/StandardProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		StandardProgram::StandardProgram()
			: ShaderProgram()
		{
			vShaderF = "shaders/default/standardprogram.vert";
			fShaderF = "shaders/default/standardprogram.frag";
		}

		StandardProgram::~StandardProgram()
		{

		}
	}
}