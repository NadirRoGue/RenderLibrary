#ifndef __RENDER_LIB_ENGINEINSTANCE_H__
#define __RENDER_LIB_ENGINEINSTANCE_H__

#include <memory>

//#include "pipeline/PipelineManager.h"

#include <string>

//#include "SceneManager.h"

namespace RenderLib
{
	class Scene;
	class SceneObject;
	class SceneManager;
	class PipelineManager;

	class EngineInstance
	{
	private:
		std::unique_ptr<PipelineManager> pipelineManager;
		std::unique_ptr<SceneManager> sceneManager;
	public:
		EngineInstance();
		~EngineInstance();

		SceneObject * createObject(SceneObject * parent = NULL, bool addToActiveScene = true);
		Scene * createScene(std::string sceneName);

		PipelineManager & getPipelineManager();
		SceneManager & getSceneManager();
	};
}

#endif