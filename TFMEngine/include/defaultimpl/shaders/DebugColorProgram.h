#ifndef __RENDERLIB_DEFAULTIMPL_DEBUGCOLORPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_DEBUGCOLORPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class DebugColorProgram : public GPU::Program::PostProcessProgram
		{
		public:
			DebugColorProgram();

			void
				onRender(GPU::Texture::GPUTexture * previousOutput,
					EngineInstance * instance);
		};
	}
}

#endif