#ifndef __RENDERLIB_DEFAULTIMPL_BLOOMPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_BLOOMPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

#include "render/FBO.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		typedef struct BloomFBOInfo
		{
			Render::FBO * fbo;
			GPU::Texture::GPUTexture * input;
		} BloomFBOInfo;

		class BloomProgram : public GPU::Program::PostProcessProgram
		{
		private:
			BloomFBOInfo fbos[2];
		public:
			BloomProgram();
			void initializeShader(EngineInstance * instance);
			void onRender(GPU::Texture::GPUTexture * previousOutput, EngineInstance * instance);
		};
	}
}

#endif