#include "pipeline/PipelineManager.h"

#include "EngineInstance.h"

// Default stages
#include "defaultimpl/components/UserScript.h"
#include "defaultimpl/pipelinestages/CPUToGPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/CPUToGPUTextureSyncStage.h"
#include "defaultimpl/pipelinestages/ComponentRegisterStage.h"
#include "defaultimpl/pipelinestages/GPUToCPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/IterationEndStage.h"
#include "defaultimpl/pipelinestages/LightSyncStage.h"
#include "defaultimpl/pipelinestages/RenderStage.h"
#include "defaultimpl/pipelinestages/ShaderCompilationStage.h"
#include "defaultimpl/pipelinestages/TransformUpdateStage.h"

namespace RenderLib
{
  namespace Pipeline
  {
    PipelineManager::PipelineManager()
    {
    }

    PipelineManager::~PipelineManager()
    {
    }

    void
    PipelineManager::setEngineInstance(EngineInstance * instance)
    {
      engineInstance = instance;
      addPipelineStage<DefaultImpl::ComponentRegisterStage>();
      addPipelineStage<DefaultImpl::CPUToGPUMeshSyncStage>();
      addPipelineStage<DefaultImpl::CPUToGPUTextureSyncStage>();
      addPipelineStage<DefaultImpl::ShaderCompilationStage>();
      addPipelineStage<ElementBasedStage<DefaultImpl::UserScript>>();
    }

    Pipeline &
    PipelineManager::getPipeline()
    {
      return pipeline;
    }

    PipelineBroker &
    PipelineManager::getStageBroker()
    {
      return stageBroker;
    }

    ThreadPool &
    PipelineManager::getThreadPool()
    {
      return threadPool;
    }

    void
    PipelineManager::registerComponent(Component * component)
    {
      stageBroker.registerElement(component);
    }

    void
    PipelineManager::removeComponent(Component * component)
    {
    }

    void
    PipelineManager::initializeStages()
    {
      addPipelineStage<DefaultImpl::TransformUpdateStage>();
      addPipelineStage<DefaultImpl::LightSyncStage>();
      addPipelineStage<DefaultImpl::RenderStage>();
      addPipelineStage<DefaultImpl::IterationEndStage>();

      for (auto & stagePtr : pipeline.getAllStages())
      {
        PipelineStage * stage = stagePtr.get();
        if (stage)
        {
          stage->preRunStage();
        }
      }
    }

    void
    PipelineManager::finishStages()
    {
      for (auto & stagePtr : pipeline.getAllStages())
      {
        PipelineStage * stage = stagePtr.get();
        if (stage)
        {
          stage->postRunStage();
        }
      }
    }

    void
    PipelineManager::executePipeline()
    {
      pipeline.execute();
    }
  } // namespace Pipeline
} // namespace RenderLib