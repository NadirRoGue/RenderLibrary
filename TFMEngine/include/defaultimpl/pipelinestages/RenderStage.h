#ifndef __RENDERLIB_DEFAULTIMPL_RENDERSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_RENDERSTAGE_H__

#include "pipeline/PipelineStage.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "render/RenderingPipeline.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class RenderStage : public Pipeline::ElementBasedStage<DefaultImpl::MeshRenderer>
		{
		private:
			Render::RenderingPipeline renderPipeline;
		public:
			void registerElement(Component * component);
			void preRunStage();
			void runStage();
			void postRunStage();

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