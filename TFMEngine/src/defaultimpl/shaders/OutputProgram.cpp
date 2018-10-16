#include "defaultimpl/shaders/OutputProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		OutputProgram::OutputProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/output.frag";
		}

		void OutputProgram::onRender(GPU::Texture::GPUTexture * previousOutput, EngineInstance * instance)
		{
			unsigned int texUnit = 0;
			setUniformTexture("inputColor", previousOutput, texUnit);
		}
	}
}