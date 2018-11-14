#ifndef __RENDERLIB_RENDER_RENDERINGPIPELINE_H__
#define __RENDERLIB_RENDER_RENDERINGPIPELINE_H__

#include <memory>
#include <vector>

#include "defaultimpl/components/MeshRenderer.h"

#include "render/AbstractRenderingStage.h"

#include "EngineException.h"
#include "EngineInstance.h"

namespace RenderLib
{
  class EngineInstance;

  namespace Render
  {
    class RenderingPipeline
    {
    private:
      AbstractRenderingStagePtr pickStage;

      std::vector<AbstractRenderingStagePtr> renderStages;
      EngineInstance * engineInstance;

    public:
      RenderingPipeline();
      ~RenderingPipeline();

      void
      setEngineInstance(EngineInstance * engineInstance);
      void
      initializeStages();
      void
      executePipelineIteration();
      void
      finalizeStages();

      void
      registerRenderable(DefaultImpl::MeshRenderer * renderable);

      template <class T>
      T *
      addRenderStage()
      {
        if (!std::is_base_of<AbstractRenderingStage, T>::value)
        {
          std::string message = "RenderingPipeline: Attempted to add a non "
                                "AbstractRenderingStage stage to the pipeline ("
              + std::string(typeid(T).name()) + ")";
          throw EngineException(message.c_str());
        }

        std::unique_ptr<AbstractRenderingStage> newStage
            = std::make_unique<T>();
        T * result = static_cast<T *>(newStage.get());

        if (renderStages.size() > 0)
        {
          AbstractRenderingStage * prev
              = renderStages[renderStages.size() - 1].get();
          if (prev)
          {
            prev->nextStage           = result;
            newStage.get()->prevStage = prev;
          }
        }

        renderStages.push_back(std::move(newStage));

        return result;
      }

      template <class T>
      void
      removeStage()
      {
        if (!std::is_base_of<AbstractRenderingStage, T>::value)
        {
          std::string message = "RenderingPipeline: Attempted to remove a non "
                                "AbstractRenderingStage stage to the pipeline ("
              + std::string(typeid(T).name()) + ")";
          throw EngineException(message.c_str());
        }

        bool found      = false;
        auto previousIt = renderStages.begin();
        for (auto it = renderStages.begin(); it != renderStages.end() && !found;
             it++)
        {
          AbstractRenderingStage * stage = (*it).get();
          if (dynamic_cast<T>(stage) != NULL)
          {
            // remove stage
						stage->finalize();
            renderStages.erase(it);
            found = true;

            it++;

            // Fix pipeline
            (*previousIt).get()->nextStage = (*it).get();
            (*it).get()->prevStage         = (*previousIt);
          }
          else
          {
            previousIt = it;
          }
        }
      }

			void
			removeAllStages();
    };
  } // namespace Render
} // namespace RenderLib

#endif