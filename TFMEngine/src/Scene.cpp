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

	void Scene::addObject(SceneObject * object)
	{
		sceneObjects.push_back(object);
	}

	const std::vector<SceneObject*> & Scene::getSceneObjects()
	{
		return sceneObjects;
	}

	void Scene::destroyScene()
	{
		for (auto so : sceneObjects)
		{
			delete so;
		}
	}
}