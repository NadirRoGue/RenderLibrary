#ifndef __RENDERLIB_DEFAULTIMPL_BLOOMPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_BLOOMPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

#include "render/FBO.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class BloomProgram : public GPU::Program::PostProcessProgram
		{
		private:
			Render::FBO * emissive1;
			Render::FBO * emissive2;
			Render::FBO * fbos[2];
		public:
			BloomProgram();
			void initializeShader(EngineInstance * instance);
			void onRender(EngineInstance * instance);
		};
	}
}

#endif