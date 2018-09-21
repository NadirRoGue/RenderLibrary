#include "render/renderstages/ForwardRenderStage.h"

namespace RenderLib
{
	namespace Render
	{
		void ForwardRenderStage::tryRegisterElement(Components::MeshRenderer * renderable)
		{
			renderables.push_back(renderable);
		}

		void ForwardRenderStage::runStage()
		{

		}
	}
}