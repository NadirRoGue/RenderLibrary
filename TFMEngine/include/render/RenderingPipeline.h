#ifndef __RENDERLIB_RENDER_RENDERINGPIPELINE_H__
#define __RENDERLIB_RENDER_RENDERINGPIPELINE_H__

#include <vector>
#include <memory>

#include "components/MeshRenderer.h"

#include "render/AbstractRenderingStage.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace Render
	{
		class RenderingPipeline
		{
		private:
			std::vector<AbstractRenderingStagePtr> renderStages;
		public:
			RenderingPipeline();
			~RenderingPipeline();

			void initializeStages(GPU::Mesh::GPUMeshManager * meshBufferManager);
			void executePipelineIteration();
			void finalizeStages();

			void registerRenderable(Components::MeshRenderer * renderable);

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

				renderStages.push_back(std::move(newStage));
			}

			template<class T>
			void removeStage()
			{
				if (!std::is_base_of<AbstractRenderingStage, T>::value)
				{
					std::string message = "RenderingPipeline: Attempted to remove a non AbstractRenderingStage stage to the pipeline (" + std::string(typeid(T).name()) + ")";
					throw EngineException(message.c_str());
				}

				for (auto it = renderStages.begin(); it != renderStages.end(); it++)
				{
					AbstractRenderingStage * stage = (*it).get();
					if (dynamic_cast<T>(stage) != NULL)
					{
						(*it).reset();
						renderStages.erase(it);
						break;
					}
				}
			}

		};
	}
}

#endif