#include "EngineInstance.h"

#include "pipeline/PipelineManager.h"
#include "SceneManager.h"

namespace RenderLib
{
	EngineInstance::EngineInstance(const std::string & instanceName)
		:instanceName(instanceName)
	{
		pipelineManager = std::make_unique<Pipeline::PipelineManager>();
		sceneManager = std::make_unique<SceneManager>();
	}

	EngineInstance::~EngineInstance()
	{
		pipelineManager.reset();
		sceneManager.reset();
	}

	const std::string & EngineInstance::getInstanceName()
	{
		return instanceName;
	}

	Pipeline::PipelineManager & EngineInstance::getPipelineManager()
	{
		return *(pipelineManager.get());
	}

	SceneManager & EngineInstance::getSceneManager()
	{
		return *(sceneManager.get());
	}
}