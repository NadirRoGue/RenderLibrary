#include "render/renderstages/DeferredShadingStage.h"

namespace RenderLib
{
	namespace Render
	{
		void DeferredShadingStage::initialize()
		{
			PostProcessRenderStage<DefaultImpl::DeferredShadingProgram>::initialize();
			outputFBO = &(FBO::DEFAULTFRAMEBUFFER);
		}
	}
}