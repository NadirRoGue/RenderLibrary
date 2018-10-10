#ifndef __RENDERLIB_RENDER_RENDERINGPIPELINE_H__
#define __RENDERLIB_RENDER_RENDERINGPIPELINE_H__

#include <vector>
#include <memory>

#include "defaultimpl/components/MeshRenderer.h"

#include "render/AbstractRenderingStage.h"

#include "EngineException.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Render
	{
		class RenderingPipeline
		{
		private:
			std::vector<AbstractRenderingStagePtr> renderStages;
			std::vector<AbstractRenderingStagePtr> postProcessStages;
		public:
			RenderingPipeline();
			~RenderingPipeline();

			void setEngineInstance(EngineInstance * engineInstance);
			void initializeStages();
			void executePipelineIteration();
			void finalizeStages();

			void registerRenderable(DefaultImpl::MeshRenderer * renderable);

			template<class T>
			T * addRenderStage()
			{
				if (!std::is_base_of<AbstractRenderingStage, T>::value)
				{
					std::string message = "RenderingPipeline: Attempted to add a non AbstractRenderingStage stage to the pipeline (" + std::string(typeid(T).name()) + ")";
					throw EngineException(message.c_str());
				}

				std::unique_ptr<T> newStage = std::make_unique<T>();
				T * result = newStage.get();

				if (std::is_base_of<MeshRenderingStage, T>::value)
				{
					renderStages.push_back(std::move(newStage));
				}
				else
				{
					postProcessStages.push_back(std::move(newStage));
				}

				return result;
			}

			template<class T>
			void removeStage()
			{
				if (!std::is_base_of<AbstractRenderingStage, T>::value)
				{
					std::string message = "RenderingPipeline: Attempted to remove a non AbstractRenderingStage stage to the pipeline (" + std::string(typeid(T).name()) + ")";
					throw EngineException(message.c_str());
				}

				bool found = false;
				for (auto it = renderStages.begin(); it != renderStages.end() && !found; it++)
				{
					AbstractRenderingStage * stage = (*it).get();
					if (dynamic_cast<T>(stage) != NULL)
					{
						//(*it).reset();
						renderStages.erase(it);
						found = true;
					}
				}

				if (!found)
				{
					for (auto it = postProcessStages.begin(); it != postProcessStages.end() && !found; it++)
					{
						AbstractRenderingStage * stage = (*it).get();
						if (dynamic_cast<T>(stage) != NULL)
						{
							//(*it).reset();
							renderStages.erase(it);
							found = true;
						}
					}
				}
			}

		};
	}
}

#endif