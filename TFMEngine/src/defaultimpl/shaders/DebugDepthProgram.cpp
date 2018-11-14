#include "defaultimpl/shaders/DebugDepthProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		DebugDepthProgram::DebugDepthProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/debugdepth.frag";
		}

		void DebugDepthProgram::onRender(GPU::Texture::GPUTexture * previousOutput,
			EngineInstance * instance)
		{
			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");
			GPU::Texture::GPUTexture * depth = gBuffers->getDepthAttachment();
			unsigned int tu = 0;
			setUniformTexture("inputTex", depth, tu);
		}
	}
}