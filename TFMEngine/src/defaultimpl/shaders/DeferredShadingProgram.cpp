#include "defaultimpl/shaders/DeferredShadingProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		DeferredShadingProgram::DeferredShadingProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/deferredshading.frag";
		}

		void DeferredShadingProgram::onRender(Scene * scene)
		{

		}
	}
}