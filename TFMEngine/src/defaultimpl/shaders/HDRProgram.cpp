#include "defaultimpl/shaders/HDRProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		HDRProgram::HDRProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/hdr.frag";
		}

		void HDRProgram::onRender(GPU::Texture::GPUTexture * previousOutput, EngineInstance * instance)
		{
			unsigned int texUnit = 0;
			setUniformTexture("inputColor", previousOutput, texUnit);
		}
	}
}