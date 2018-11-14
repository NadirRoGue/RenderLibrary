#ifndef __RENDERLIB_DEFAULTIMPL_DEBUGPOSITIONPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_DEBUGPOSITIONPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class DebugPositionProgram : public GPU::Program::PostProcessProgram
		{
		public:
			DebugPositionProgram();

			void
			onRender(GPU::Texture::GPUTexture * previousOutput,
				EngineInstance * instance);
		};
	}
}

#endif