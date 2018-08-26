#ifndef __CPU_SCENE_H__
#define __CPU_SCENE_H__

#include <map>
#include <vector>
#include <typeinfo>

#include "Program.h"
#include "SceneObject.h"

namespace RenderLib
{
	class Scene
	{
	private:
		std::vector<SceneObject *> sceneObjects;

	public:
		Scene();
		~Scene();

		void addObject(SceneObject * object);

		const std::vector<SceneObject *> & getSceneObjects();
	};
}

#endif