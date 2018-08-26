#include "Scene.h"

namespace RenderLib
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::addObject(SceneObject * object)
	{
		sceneObjects.push_back(object);
	}

	const std::vector<SceneObject*> & Scene::getSceneObjects()
	{
		return sceneObjects;
	}
}