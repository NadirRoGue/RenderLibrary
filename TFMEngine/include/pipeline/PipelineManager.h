#ifndef __RENDERLIB_PIPELINEMANAGER_H__
#define __RENDERLIB_PIPELINEMANAGER_H__

#include "pipeline/Pipeline.h"
#include "pipeline/PipelineBroker.h"
#include "pipeline/Threadpool.h"

#include "Component.h"

#include "Scene.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Pipeline
	{
		class PipelineManager
		{
		private:
			EngineInstance * engineInstance;
			Pipeline pipeline;
			PipelineBroker stageBroker;
			ThreadPool threadPool;

		public:
			PipelineManager();
			~PipelineManager();

			void setEngineInstance(EngineInstance * instance);

			Pipeline & getPipeline();
			PipelineBroker & getStageBroker();
			ThreadPool & getThreadPool();

			template<class T>
			T * addPipelineStage()
			{
				T * result = pipeline.registerStage<T>();
				PipelineStage * stage = dynamic_cast<PipelineStage*>(result);
				stage->engineInstance = engineInstance;
				if (result && stage)
				{
					stageBroker.registerPipelineStage(result);
				}

				return result;
			}

			template<class T>
			T * getPipelineStage()
			{
				return pipeline.getStage<T>();
			}

			void registerComponent(Component * component);
			void removeComponent(Component * component);

			void initializeStages();

			void finishStages();

			void executePipeline();
		};
	}
}

#endif