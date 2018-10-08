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

	Lighting::DirectionalLight * Scene::addDirectionalLight(const std::string & name)
	{
		std::unique_ptr<SceneObject> newDL = std::make_unique<Lighting::DirectionalLight>();
		Lighting::DirectionalLight * result = static_cast<Lighting::DirectionalLight*>(newDL.get());
		
		newDL.get()->objectName = name;

		sceneTreeUpdated = true;

		direcionalLights.push_back(result);
		sceneObjects.push_back(std::move(newDL));

		return result;
	}

	Lighting::SpotLight * Scene::addSpotLight(const std::string & name)
	{
		std::unique_ptr<SceneObject> newSL = std::make_unique<Lighting::SpotLight>();
		Lighting::SpotLight * result = static_cast<Lighting::SpotLight*>(newSL.get());

		newSL.get()->objectName = name;

		sceneTreeUpdated = true;

		spotLights.push_back(result);
		sceneObjects.push_back(std::move(newSL));

		return result;
	}

	Lighting::PointLight * Scene::addPointLight(const std::string & name)
	{
		std::unique_ptr<SceneObject> newPL = std::make_unique<Lighting::PointLight>();
		Lighting::PointLight * result = static_cast<Lighting::PointLight*>(newPL.get());

		newPL.get()->objectName = name;

		sceneTreeUpdated = true;

		pointLights.push_back(result);
		sceneObjects.push_back(std::move(newPL));

		return result;
	}

	std::vector<Lighting::DirectionalLight*> & Scene::getDirectionalLights()
	{
		return direcionalLights;
	}

	std::vector<Lighting::SpotLight*> & Scene::getSpotLights()
	{
		return spotLights;
	}

	std::vector<Lighting::PointLight*> & Scene::getPointLights()
	{
		return pointLights;
	}
}