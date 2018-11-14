#include "defaultimpl/shaders/DebugColorProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		DebugColorProgram::DebugColorProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/debugcolor.frag";
		}

		void DebugColorProgram::onRender(GPU::Texture::GPUTexture * previousOutput,
			EngineInstance * instance)
		{
			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");
			GPU::Texture::GPUTexture * color = gBuffers->getAttachment("diffuseAndOpacity");
			unsigned int tu = 0;
			setUniformTexture("inputTex", color, tu);
		}
	}
}