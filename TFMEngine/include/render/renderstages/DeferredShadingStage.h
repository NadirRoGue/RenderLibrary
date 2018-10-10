#ifndef __RENDERLIB_RENDER_DEFERREDSHADINGSTAGE_H__
#define __RENDERLIB_RENDER_DEFERREDSHADINGSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "defaultimpl/shaders/DeferredShadingProgram.h"

namespace RenderLib
{
	namespace Render
	{
		class DeferredShadingStage : 
			public PostProcessRenderStage<DefaultImpl::DeferredShadingProgram>
		{
		public:
			void initialize();
		};
	}
}

#endif