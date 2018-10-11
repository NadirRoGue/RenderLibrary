#ifndef __RENDERLIB_RENDER_BLOOMPOSTPROCESSSTAGE_H__
#define __RENDERLIB_RENDER_BLOOMPOSTPROCESSSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "defaultimpl/shaders/BloomProgram.h"

namespace RenderLib
{
	namespace Render
	{
		class BloomPostProcessStage : public PostProcessRenderStage<DefaultImpl::BloomProgram>
		{
		public:
			void initialize();
		};
	}
}

#endif