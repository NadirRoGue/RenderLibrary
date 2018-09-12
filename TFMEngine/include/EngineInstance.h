#ifndef __RENDER_LIB_ENGINEINSTANCE_H__
#define __RENDER_LIB_ENGINEINSTANCE_H__

#include <memory>
#include <string>

namespace RenderLib
{
	class SceneManager;

	namespace Pipeline
	{
		class PipelineManager;
	}

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
	};
}

#endif