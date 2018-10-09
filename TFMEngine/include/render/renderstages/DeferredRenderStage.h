#ifndef __RENDERLIB_RENDER_DEFERREDRENDERSTAGE_H__
#define __RENDERLIB_RENDER_DEFERREDRENDERSTAGE_H__

#include "render/AbstractRenderingStage.h"

namespace RenderLib
{
	namespace Render
	{
		class DeferredRenderStage : public MeshRenderingStage
		{
		private:
			FBO * gBuffers;
		public:
			DeferredRenderStage();
			bool shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
			void runStage();
		};
	}
}

#endif