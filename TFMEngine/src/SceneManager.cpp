#include "SceneManager.h"

#include <iostream>

namespace RenderLib
{
	SceneManager::SceneManager()
	{

	}

	SceneManager::~SceneManager()
	{

	}

	Scene * SceneManager::createScene(const std::string & sceneName)
	{
		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);

		Scene * scene = NULL;

		if (it != sceneCache.end())
		{
			return NULL; // FIXME: Throw exception
		}

		std::unique_ptr<Scene> newScene = std::make_unique<Scene>(sceneName);
		scene = newScene.get();
		sceneCache[sceneName] = std::move(newScene);
		//scene = sceneCache[sceneName].get();
		
		return scene;
	}

	void SceneManager::deleteScene(const std::string & sceneName)
	{
		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);
		if (it != sceneCache.end())
		{
			it->second.reset();

			sceneCache.erase(it);
		}
	}

	Scene * SceneManager::getScene(const std::string & sceneName)
	{
		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);
		if (it == sceneCache.end())
		{
			return NULL;
		}

		return it->second.get();
	}

	Scene * SceneManager::getActiveScene()
	{
		return activeScene;
	}

	void SceneManager::setActiveScene(std::string sceneName)
	{
		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);
		if (it != sceneCache.end())
		{
			activeScene = it->second.get();
		}
	}
}