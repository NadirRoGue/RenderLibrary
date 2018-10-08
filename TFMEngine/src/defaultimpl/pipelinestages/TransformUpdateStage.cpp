#include "defaultimpl/pipelinestages/TransformUpdateStage.h"

#include <queue>
#include <iostream>

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void TransformUpdateStage::updateLinealGraph()
		{
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			
			std::queue<SceneObject*> toProcess;
			std::queue<SceneObject*> tempQueue;

			// Add initial batch: scene's root children
			for (auto rootChild : scene->getSceneRoot()->getChildren())
			{
				tempQueue.push(rootChild);
			}

			// Updates graph by level, ensuring children will have access
			// to parent's UPDATED transforms
			linearGraph.clear();
			do
			{
				// Change queues: previous iteration added children will be
				// updated parents now
				toProcess.swap(tempQueue);

				std::vector<Component*> treeLevel;

				while (!toProcess.empty())
				{
					SceneObject * next = toProcess.front();
					toProcess.pop();

					treeLevel.push_back(&(next->transform));

					if (next->getChildren().size() > 0)
					{
						for (auto child : next->getChildren())
						{
							tempQueue.push(child);
						}
					}
				}

				// Push current level
				linearGraph.push_back(treeLevel);

			} while (!tempQueue.empty());

			scene->setSceneTreeNeedsUpdate(false);
		}

		void TransformUpdateStage::preRunStage()
		{
			updateLinealGraph();
		}

		void TransformUpdateStage::runStage()
		{
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			if (scene->sceneTreeNeedsUpdate())
			{
				updateLinealGraph();
			}

			for (auto & treeLevel : linearGraph)
			{
				elements.clear();
				elements = treeLevel;
				ElementBasedStage<Transform>::runStage();
			}
		}

		void TransformUpdateStage::processElement(Transform * element)
		{
			element->updateGraph();
		}
	}
}