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

	const std::vector<SceneObjectPtr> & Scene::getSceneObjects()
	{
		return sceneObjects;
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