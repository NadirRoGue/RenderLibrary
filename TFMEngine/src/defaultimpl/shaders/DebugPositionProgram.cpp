#include "defaultimpl/shaders/DebugPositionProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		DebugPositionProgram::DebugPositionProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/debugpos.frag";
		}

		void DebugPositionProgram::onRender(GPU::Texture::GPUTexture * previousOutput,
			EngineInstance * instance)
		{
			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");
			GPU::Texture::GPUTexture * pos = gBuffers->getAttachment("position");
			unsigned int tu = 0;
			setUniformTexture("inputTex", pos, tu);
		}
	}
}