#ifndef __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__
#define __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__

#include <memory>

namespace RenderLib
{
	namespace Render
	{
		class AbstractRenderingStage
		{
		public:
			virtual void initialize();
			virtual void runStage() = 0;
			virtual void finalize();
		};

		typedef std::unique_ptr<AbstractRenderingStage> AbstractRenderingStagePtr;
	}
}

#endif