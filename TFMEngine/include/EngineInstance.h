#ifndef __RENDER_LIB_ENGINEINSTANCE_H__
#define __RENDER_LIB_ENGINEINSTANCE_H__

#include <memory>
#include <string>

#include "SceneManager.h"
#include "pipeline/PipelineManager.h"

namespace RenderLib
{
	class EngineInstance
	{
	private:
		std::string instanceName;

		std::unique_ptr<Pipeline::PipelineManager> pipelineManager;
		std::unique_ptr<SceneManager> sceneManager;
	public:
		EngineInstance(const std::string & instanceName);
		~EngineInstance();

		const std::string & getInstanceName();

		Pipeline::PipelineManager & getPipelineManager();
		SceneManager & getSceneManager();

		void launchExecution();
	private:
		void initialize();
		void execute();
		void finalize();
	};
}

#endif