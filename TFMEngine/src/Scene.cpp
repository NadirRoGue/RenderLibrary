#include "Scene.h"

#include "logger/Log.h"

namespace RenderLib
{
	Scene::Scene()
		: activeCamera(NULL)
	{
		sceneRoot = addObject<SceneObject>();
	}

	Scene::Scene(std::string sceneName)
		:name(sceneName)
	{

	}

	Scene::~Scene()
	{
	}

	const std::string & Scene::getSceneName()
	{
		return name;
	}

	void Scene::makeCameraActive(const std::string & name)
	{
		auto it = sceneCameras.find(name);
		if (it != sceneCameras.end())
		{
			activeCamera = it->second;
		}
		else
		{
			Logger::Log::getInstance().logWarning("Scene: Attempt to set camera " + name + " as active failed: No such camera");
		}
	}

	void Scene::makeCameraActive(Camera * cam)
	{
		activeCamera = cam;
	}

	Camera * Scene::getActiveCamera()
	{
		return activeCamera;
	}

	SceneObject * Scene::getSceneRoot()
	{
		return sceneRoot;
	}

	std::vector<SceneObjectPtr> & Scene::getSceneObjects()
	{
		return sceneObjects;
	}
	
	std::vector<Graphics::WindowResizeObserver*> & Scene::getWindowResizableObservers()
	{
		return windowResizables;
	}
	
	InputHandlers::InputManager & Scene::getInputManager()
	{
		return inputManager;
	}
}