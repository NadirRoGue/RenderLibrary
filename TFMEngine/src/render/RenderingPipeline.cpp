#include "render/RenderingPipeline.h"

namespace RenderLib
{
	namespace Render
	{
		RenderingPipeline::RenderingPipeline()
		{

		}

		RenderingPipeline::~RenderingPipeline()
		{

		}

		void RenderingPipeline::registerRenderable(Components::MeshRenderer * renderable)
		{
			for (auto & stages : renderStages)
			{
				stages.get()->tryRegisterElement(renderable);
			}
		}

		void RenderingPipeline::initializeStages()
		{
			for (auto & stages : renderStages)
			{
				stages.get()->initialize();
			}
		}

		void RenderingPipeline::executePipelineIteration()
		{
			for (auto & stages : renderStages)
			{
				stages.get()->runStage();
			}
		}

		void RenderingPipeline::finalizeStages()
		{
			for (auto & stages : renderStages)
			{
				stages.get()->finalize();
			}
		}
	}
}
