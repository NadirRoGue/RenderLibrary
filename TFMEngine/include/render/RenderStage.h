#ifndef __RENDERLIB_RENDER_RENDERSTAGE_H__
#define __RENDERLIB_RENDER_RENDERSTAGE_H__

#include "pipeline/PipelineStage.h"

#include "components/MeshRenderer.h"

#include "render/RenderingPipeline.h"

namespace RenderLib
{
	namespace Render
	{
		class RenderStage : public Pipeline::ElementBasedStage<Components::MeshRenderer>
		{
		private:
			RenderingPipeline renderPipeline;
		public:
			void processElement(Components::MeshRenderer * meshRenderer);

			template<class T>
			T * addRenderStage()
			{
				return renderPipeline.addRenderStage<T>();
			}

			template<class T>
			void removeRenderStage()
			{
				renderPipeline.removeStage<T>();
			}
		};
	}
}

#endif