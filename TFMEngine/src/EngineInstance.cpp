#include "EngineInstance.h"

#include <iostream>

#include "pipeline/PipelineManager.h"

#include "SceneObject.h"
#include "SceneManager.h"

namespace RenderLib
{
	EngineInstance::EngineInstance()
	{
		pipelineManager = std::make_unique<PipelineManager>();
		sceneManager = std::make_unique<SceneManager>();
	}

	EngineInstance::~EngineInstance()
	{
		pipelineManager.reset();
		sceneManager.reset();
	}

	SceneObject * EngineInstance::createObject(SceneObject * parent, bool addToActiveScene)
	{
		SceneObject * newObject = new SceneObject(parent);
		newObject->setEngine(this);

		if (addToActiveScene)
		{
			Scene * active = sceneManager.get()->getActiveScene();
			if (active != NULL)
			{
				active->addObject(newObject);
			}
		}

		return newObject;
	}

	Scene * EngineInstance::createScene(std::string sceneName)
	{
		return sceneManager.get()->createScene(sceneName);
	}

	PipelineManager & EngineInstance::getPipelineManager()
	{
		return *(pipelineManager.get());
	}

	SceneManager & EngineInstance::getSceneManager()
	{
		return *(sceneManager.get());
	}
}