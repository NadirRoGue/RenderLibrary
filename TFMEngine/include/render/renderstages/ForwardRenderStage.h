#ifndef __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__
#define __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include <vector>

namespace RenderLib
{
	namespace Render
	{
		class ForwardRenderStage : public AbstractRenderingStage
		{
		private:
			std::vector<Components::MeshRenderer*> renderables;

			unsigned int programId;
			unsigned int mvp;
			unsigned int apos;
		public:
			void initialize();
			void tryRegisterElement(Components::MeshRenderer * renderable);
			void runStage();
		};
	}
}

#endif