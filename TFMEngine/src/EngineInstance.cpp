#include "EngineInstance.h"

#include "pipeline/PipelineManager.h"

#include "graphics/ContextManager.h"

#include "SceneManager.h"
#include "EngineException.h"

namespace RenderLib
{
	EngineInstance::EngineInstance(const unsigned int & ID, const std::string & instanceName, Graphics::WindowHandler * handler)
		: instanceID(ID)
		, instanceName(instanceName)
		, window(handler)
		, enableFlag(true)
	{
		if (!handler)
		{
			throw EngineException(("EngineInstance: Failed constructing instance " + instanceName + ", WindowHandler is NULL").c_str());
		}

		pipelineManager = std::make_unique<Pipeline::PipelineManager>(this);
		//sceneManager = std::make_unique<SceneManager>();
		//gpuMeshManager = std::make_unique<GPU::Mesh::GPUMeshManager>();
		window->instance = this;
	}

	EngineInstance::~EngineInstance()
	{
		pipelineManager.reset();
		//sceneManager.reset();
	}

	const unsigned int & EngineInstance::getInstanceID()
	{
		return instanceID;
	}

	const std::string & EngineInstance::getInstanceName()
	{
		return instanceName;
	}

	Graphics::WindowHandler * EngineInstance::getWindow()
	{
		return window;
	}

	GPU::Mesh::GPUMeshManager & EngineInstance::getGPUMeshManager()
	{
		return gpuMeshManager;// *(gpuMeshManager.get());
	}

	void EngineInstance::loadActiveScene()
	{
		Scene * scene = sceneManager.getActiveScene();// sceneManager.get()->getActiveScene();
		pipelineManager.get()->initializeStages();
	}
	
	void EngineInstance::loadScene(const std::string & sceneName)
	{
		Scene * scene = sceneManager.getScene(sceneName);// sceneManager.get()->getScene(sceneName);
		if (scene)
		{
			sceneManager.setActiveScene(sceneName);//sceneManager.get()->setActiveScene(sceneName);
			loadActiveScene();
		}
		else
		{
			std::string message = "EngineInstance [name=" + instanceName + ", ID=" + std::to_string(instanceID) + "]: Failed to load scene " + sceneName;
			throw EngineException(message.c_str());
		}
	}

	void EngineInstance::disable()
	{
		enableFlag = false;
	}

	bool EngineInstance::isEnabled()
	{
		return enableFlag;
	}

	Pipeline::PipelineManager & EngineInstance::getPipelineManager()
	{
		return *(pipelineManager.get());
	}

	SceneManager & EngineInstance::getSceneManager()
	{
		return sceneManager;// *(sceneManager.get());
	}

	GPU::Program::ProgramManager & EngineInstance::getProgramManager()
	{
		return gpuProgramManager;
	}

	void EngineInstance::executeIteration()
	{
		// Execute pipeline stages
		pipelineManager.get()->executePipeline();

		// Swap buffers
		acquireContext();
		window->onRenderLoopIteration();
		releaseContext();
	}

	void EngineInstance::acquireContext()
	{
		Graphics::ContextManager().getInstance().aquireContext();
		window->activateContext();
	}

	void EngineInstance::releaseContext()
	{
		Graphics::ContextManager::getInstance().releaseContext();
	}

	void EngineInstance::cleanUp()
	{
		pipelineManager.get()->finishStages();
		window->cleanUp();
	}
}