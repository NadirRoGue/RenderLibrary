#include "SceneManager.h"

namespace RenderLib
{
	SceneManager::SceneManager()
	{

	}

	SceneManager::~SceneManager()
	{

	}

	Scene * SceneManager::createScene(const std::string & sceneName, unsigned int & result)
	{
		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);

		result = CREATE_CODE_SUCCESS;
		Scene * scene = NULL;

		if (it != sceneCache.end())
		{
			result |= CREATE_CODE_DUPLICATE_NAME;
		}

		if(result == CREATE_CODE_SUCCESS)
		{
			std::unique_ptr<Scene> newScene = std::make_unique<Scene>(sceneName);
			sceneCache[sceneName] = std::move(newScene);
			scene = newScene.get();
		}

		return scene;
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

	void SceneManager::setActiveScene(std::string sceneName, unsigned int & result)
	{
		result = ACTIVATE_CODE_SUCCESS;

		std::map<std::string, std::unique_ptr<Scene>>::iterator it = sceneCache.find(sceneName);
		if (it == sceneCache.end())
		{
			result |= ACTIVATE_CODE_SCENE_NOT_FOUND;
		}
		
		if (result == ACTIVATE_CODE_SUCCESS)
		{
			activeScene = it->second.get();
		}
	}
}