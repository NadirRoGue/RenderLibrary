#ifndef __RENDERLIB_PIPELINEMANAGER_H__
#define __RENDERLIB_PIPELINEMANAGER_H__

#include "pipeline/Pipeline.h"
#include "pipeline/PipelineBroker.h"

#include "Component.h"

namespace RenderLib
{
	namespace Pipeline
	{
		class PipelineManager
		{
		private:
			Pipeline pipeline;
			PipelineBroker stageBroker;

		public:
			PipelineManager();
			~PipelineManager();

			const Pipeline & getPipeline();
			const PipelineBroker & getStageBroker();

			template<class T>
			T * addPipelineStage()
			{
				T * result = pipeline.registerStage<T>();
				PipelineStage * stage = dynamic_cast<PipelineStage*>(result);
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

			void executePipeline();
		};
	}
}

#endif