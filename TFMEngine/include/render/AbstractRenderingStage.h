#ifndef __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__
#define __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__

#include <memory>

#include "components/MeshRenderer.h"

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
			virtual void tryRegisterElement(Components::MeshRenderer * renderable);
		};

		typedef std::unique_ptr<AbstractRenderingStage> AbstractRenderingStagePtr;
	}
}

#endif