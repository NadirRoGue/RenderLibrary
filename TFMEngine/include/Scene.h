#ifndef __CPU_SCENE_H__
#define __CPU_SCENE_H__

#include <map>
#include <vector>
#include <typeinfo>
#include <memory>

#include "Program.h"
#include "SceneObject.h"

#include "inputhandlers/InputManager.h"

namespace RenderLib
{
	class Scene
	{
	private:
		std::string name;
		std::vector<SceneObjectPtr> sceneObjects;

		InputHandlers::InputManager inputManager;
	public:
		Scene();
		Scene(std::string name);
		~Scene();

		const std::string & getSceneName();

		void addObject(SceneObjectPtr & object);

		const std::vector<SceneObjectPtr> & getSceneObjects();

		InputHandlers::InputManager & getInputManager();

		void destroyScene();
	};
}

#endif