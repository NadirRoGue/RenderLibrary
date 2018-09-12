#include "EngineInstance.h"

#include "pipeline/PipelineManager.h"
#include "SceneManager.h"

#include "EngineException.h"

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

	void EngineInstance::launchExecution()
	{

	}

	void EngineInstance::initialize()
	{
		Scene * activeScene = getSceneManager().getActiveScene();

		if (!activeScene)
		{
			throw EngineException(("EngineInstance [" + instanceName + "]: no active scene was found. Aborting execution").c_str());
		}

		// Register all components in the appropiate place
		auto objectList = activeScene->getSceneObjects();
		for (auto & obj : objectList)
		{
			SceneObject * object = obj.get();
			if (object)
			{
				for (auto & objComp : object->getAllComponents())
				{
					pipelineManager.get()->registerComponent(objComp.get());
				}
			}
		}

		// Do the same with cameras
		auto cameraList = activeScene->getAllCameras();
		for (auto & camPtr : cameraList)
		{
			Camera * cam = camPtr.get();
			if (cam)
			{
				for (auto & objComp : cam->getAllComponents())
				{
					pipelineManager.get()->registerComponent(objComp.get());
				}
			}
		}

		// Optimize stage execution
		pipelineManager.get()->initializeStages();
	}

	void EngineInstance::execute()
	{

	}

	void EngineInstance::finalize()
	{
		pipelineManager.get()->finishStages();
	}
}