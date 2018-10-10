#ifndef __RENDERLIB_RENDER_DEFERREDRENDERSTAGE_H__
#define __RENDERLIB_RENDER_DEFERREDRENDERSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "defaultimpl/shaders/DeferredShadingProgram.h"

namespace RenderLib
{
	namespace Render
	{
		class DeferredRenderStage : public MeshRenderingStage
		{
		private:
			FBO * gBuffers;
			DefaultImpl::DeferredShadingProgram * deferredShading;
		public:
			DeferredRenderStage();
			void initialize();
			bool shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
			void runStage();
		};
	}
}

#endif