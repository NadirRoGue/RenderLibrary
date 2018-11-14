#include "render/RenderingPipeline.h"

#include "render/renderstages/DeferredRenderStage.h"
#include "render/renderstages/ForwardRenderStage.h"
#include "render/renderstages/PickingRenderStage.h"
#include "render/renderstages/PipelineOutputStage.h"

#include "defaultimpl/shaders/BloomProgram.h"
#include "defaultimpl/shaders/HDRProgram.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace Render
  {
    RenderingPipeline::RenderingPipeline()
    {
      pickStage = std::make_unique<PickingRenderStage>();

      addRenderStage<DeferredRenderStage>();
      addRenderStage<ForwardRenderStage>();
      addRenderStage<PostProcessRenderStage<DefaultImpl::BloomProgram>>();
      addRenderStage<PostProcessRenderStage<DefaultImpl::HDRProgram>>();
    }

    RenderingPipeline::~RenderingPipeline()
    {
    }

    void
    RenderingPipeline::registerRenderable(
        DefaultImpl::MeshRenderer * renderable)
    {
      pickStage.get()->tryRegisterElement(renderable);

      for (auto & stages : renderStages)
      {
        stages.get()->tryRegisterElement(renderable);
      }
    }

    void
    RenderingPipeline::setEngineInstance(EngineInstance * engineInstance)
    {
      pickStage.get()->engineInstance = engineInstance;

      addRenderStage<PipelineOutputStage>();

      this->engineInstance = engineInstance;
      for (auto & stage : renderStages)
      {
        stage.get()->engineInstance = engineInstance;
      }
    }

    void
    RenderingPipeline::initializeStages()
    {
      pickStage.get()->initialize();

      for (auto & stages : renderStages)
      {
        stages.get()->initialize();
      }
    }

    void
    RenderingPipeline::executePipelineIteration()
    {
      pickStage.get()->runStage();

      for (auto & stages : renderStages)
      {
        stages.get()->runStage();
      }
    }

    void
    RenderingPipeline::finalizeStages()
    {
      pickStage.get()->finalize();

      for (auto & stages : renderStages)
      {
        stages.get()->finalize();
      }
    }

		void RenderingPipeline::removeAllStages()
		{
			finalizeStages();
			renderStages.clear();
		}
  } // namespace Render
} // namespace RenderLib
