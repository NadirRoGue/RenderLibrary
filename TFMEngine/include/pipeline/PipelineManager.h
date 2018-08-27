#ifndef __RENDERLIB_PIPELINEMANAGER_H__
#define __RENDERLIB_PIPELINEMANAGER_H__

#include "pipeline/Pipeline.h"
#include "pipeline/PipelineBroker.h"

#include "Component.h"

namespace RenderLib
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
			return pipeline.registerStage<T>();
		}

		template<class T>
		T * getPipelineStage()
		{
			return pipeline.getStage<T>();
		}

		void registerComponent(Component * component);

		void executePipeline();
	};
}

#endif