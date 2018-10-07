#include "Scene.h"

#include "logger/Log.h"

namespace RenderLib
{
	Scene::Scene(std::string sceneName)
		: name(sceneName)
		, activeCamera(NULL)
		, sceneTreeUpdated(false)
	{
		sceneRoot = addObject<SceneObject>();
		sceneRoot->objectName = "Root";
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

	bool Scene::sceneTreeNeedsUpdate()
	{
		return sceneTreeUpdated;
	}

	void Scene::setSceneTreeNeedsUpdate(const bool & val)
	{
		sceneTreeUpdated = val;
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

	Lighting::DirectionalLight * Scene::addDirectionalLight()
	{
		std::unique_ptr<SceneObject> newDL = std::make_unique<Lighting::DirectionalLight>();
		Lighting::DirectionalLight * result = static_cast<Lighting::DirectionalLight*>(newDL.get());
		
		direcionalLights.push_back(result);
		sceneObjects.push_back(std::move(newDL));

		return result;
	}

	std::vector<Lighting::DirectionalLight*> & Scene::geteDirectionalLights()
	{
		return direcionalLights;
	}
}