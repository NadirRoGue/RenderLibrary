#ifndef __RENDERLIB_DEFAULTIMPL_DEBUGDEPTHPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_DEBUGDEPTHPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class DebugDepthProgram : public GPU::Program::PostProcessProgram
		{
		public:
			DebugDepthProgram();

			void
				onRender(GPU::Texture::GPUTexture * previousOutput,
					EngineInstance * instance);
		};
	}
}

#endif