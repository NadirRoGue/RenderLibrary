#ifndef __CPU_SCENE_H__
#define __CPU_SCENE_H__

#include <map>
#include <vector>
#include <typeinfo>
#include <memory>

#include "Program.h"
#include "SceneObject.h"
#include "Camera.h"

#include "inputhandlers/InputManager.h"

namespace RenderLib
{
	class Scene
	{
	private:
		std::string name;
		std::vector<SceneObjectPtr> sceneObjects;
		std::vector<CameraPtr> cameras;

		InputHandlers::InputManager inputManager;
	public:
		Scene();
		Scene(std::string name);
		~Scene();

		const std::string & getSceneName();

		void addObject(SceneObjectPtr & object);

		void addCamera(CameraPtr & camera);

		std::vector<SceneObjectPtr> & getSceneObjects();

		std::vector<CameraPtr> & getAllCameras();

		InputHandlers::InputManager & getInputManager();

		void destroyScene();
	};
}

#endif