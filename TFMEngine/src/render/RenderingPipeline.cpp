#include "render/RenderingPipeline.h"

#include "render/renderstages/ForwardRenderStage.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		RenderingPipeline::RenderingPipeline()
		{
			addRenderStage<ForwardRenderStage>();
		}

		RenderingPipeline::~RenderingPipeline()
		{

		}

		void RenderingPipeline::registerRenderable(DefaultImpl::MeshRenderer * renderable)
		{
			for (auto & stages : renderStages)
			{
				stages.get()->tryRegisterElement(renderable);
			}
		}

		void RenderingPipeline::setEngineInstance(EngineInstance * engineInstance)
		{
			for (auto & stage : renderStages)
			{
				stage.get()->engineInstance = engineInstance;
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
