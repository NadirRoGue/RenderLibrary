#include "defaultimpl/pipelinestages/TransformUpdateStage.h"

#include <queue>

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void TransformUpdateStage::buildLinearSceneTree()
		{
			elements.clear();
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			elements.reserve(scene->getSceneObjects().size());

			std::queue<SceneObject*> toProcess;
			for (auto rootChild : scene->getSceneRoot()->getChildren())
			{
				toProcess.push(rootChild);
			}

			while (!toProcess.empty())
			{
				SceneObject * next = toProcess.front();
				toProcess.pop();

				elements.push_back(&(next->transform));

				if (next->getChildren().size() > 0)
				{
					for (auto child : next->getChildren())
					{
						toProcess.push(child);
					}
				}
			}
		}

		void TransformUpdateStage::preRunStage()
		{
			buildLinearSceneTree();
		}

		void TransformUpdateStage::runStage()
		{
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			if (scene->sceneTreeNeedsUpdate())
			{
				buildLinearSceneTree();
				scene->setSceneTreeNeedsUpdate(false);
			}

			ElementBasedStage<Transform>::runStage();
		}

		void TransformUpdateStage::processElement(Transform * element)
		{
			element->updateGraph();
		}
	}
}