#include "Scene.h"

namespace RenderLib
{
	Scene::Scene()
	{

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

	void Scene::addObject(SceneObjectPtr & object)
	{
		sceneObjects.push_back(std::move(object));
	}

	void Scene::addCamera(CameraPtr & camera)
	{
		Camera * cam = camera.get();
		sceneObjects.push_back(std::move(camera));
		cameras.push_back(cam);
	}

	std::vector<SceneObjectPtr> & Scene::getSceneObjects()
	{
		return sceneObjects;
	}

	std::vector<Camera*> & Scene::getAllCameras()
	{
		return cameras;
	}

	InputHandlers::InputManager & Scene::getInputManager()
	{
		return inputManager;
	}

	void Scene::destroyScene()
	{
		for (auto & so : sceneObjects)
		{
			so.reset();
		}

		sceneObjects.clear();
	}
}