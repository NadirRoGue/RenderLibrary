#include "render/RenderingPipeline.h"

#include "render/renderstages/ForwardRenderStage.h"

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

		void RenderingPipeline::registerRenderable(Components::MeshRenderer * renderable)
		{
			for (auto & stages : renderStages)
			{
				stages.get()->tryRegisterElement(renderable);
			}
		}

		void RenderingPipeline::initializeStages(GPU::Mesh::GPUMeshManager * meshBufferManager)
		{
			for (auto & stages : renderStages)
			{
				stages.get()->gpuMeshManager = meshBufferManager;
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
