#ifndef __RENDERLIB_SCENEMANAGER_H__
#define __RENDERLIB_SCENEMANAGER_H__

#include <map>
#include <memory>

#include "Scene.h"

namespace RenderLib
{
	class SceneManager
	{
	public:
		static unsigned int CREATE_CODE_SUCCESS;
		static unsigned int CREATE_CODE_DUPLICATE_NAME;

		static unsigned int ACTIVATE_CODE_SUCCESS;
		static unsigned int ACTIVATE_CODE_SCENE_NOT_FOUND;
	private:
		std::map<std::string, std::unique_ptr<Scene>> sceneCache;

		Scene * activeScene;

	public:
		SceneManager();
		~SceneManager();

		Scene * createScene(const std::string & sceneName, unsigned int & result);
		Scene * getScene(const std::string & sceneName);
		Scene * getActiveScene();
		void setActiveScene(std::string sceneName, unsigned int & result);
	};
}

#endif