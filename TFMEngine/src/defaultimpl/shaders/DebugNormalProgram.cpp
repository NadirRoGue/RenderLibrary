#include "defaultimpl/shaders/DebugNormalProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		DebugNormalProgram::DebugNormalProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/debugnormal.frag";
		}

		void DebugNormalProgram::onRender(GPU::Texture::GPUTexture * previousOutput,
			EngineInstance * instance)
		{
			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");
			GPU::Texture::GPUTexture * normals = gBuffers->getAttachment("normals");
			unsigned int tu = 0;
			setUniformTexture("inputTex", normals, tu);
		}
	}
}