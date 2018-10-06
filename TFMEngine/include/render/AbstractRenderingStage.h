#ifndef __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__
#define __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__

#include <memory>

#include "defaultimpl/components/MeshRenderer.h"

#include "render/RenderableMap.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Render
	{
		class AbstractRenderingStage
		{
		public:
			EngineInstance * engineInstance;
		public:
			virtual void initialize();
			virtual void runStage() = 0;
			virtual void finalize();
			virtual void tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
			virtual void forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
		};

		typedef std::unique_ptr<AbstractRenderingStage> AbstractRenderingStagePtr;

		class MeshRenderingStage : public AbstractRenderingStage
		{
		protected:
			RenderableMap staticRenderables;
			RenderableMap dynamicRenderables;
		public:
			void initialize();
			void tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
			void forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);

			virtual bool shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable) = 0;
		};
	}
}

#endif