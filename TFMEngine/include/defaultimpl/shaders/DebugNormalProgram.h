#ifndef __RENDERLIB_DEFAULTIMPL_DEBUGNORMALPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_DEBUGNORMALPROGRAM_H__

#include "GPU/program/PostProcessProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class DebugNormalProgram : public GPU::Program::PostProcessProgram
		{
		public:
			DebugNormalProgram();

			void
				onRender(GPU::Texture::GPUTexture * previousOutput,
					EngineInstance * instance);
		};
	}
}

#endif