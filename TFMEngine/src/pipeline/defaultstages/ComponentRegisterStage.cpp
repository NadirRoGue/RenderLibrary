#include "pipeline/defaultstages/ComponentRegisterStage.h"

#include "pipeline/PipelineManager.h"

#include <iostream>

namespace RenderLib
{
	namespace Pipeline
	{
		void ComponentRegisterStage::preRunStage()
		{
			PipelineBroker & stageBroker = pipelineManager->getStageBroker();

			std::vector<SceneObjectPtr> & objects = scene->getSceneObjects();
			for (auto & objPtr : objects)
			{
				SceneObject * obj = objPtr.get();
				
				std::vector<std::unique_ptr<Component>> & unregisterComps = obj->getComponentList().getUnregisteredComponents();
				if (unregisterComps.size() > 0)
				{
					for (auto & unregisteredComp : unregisterComps)
					{
						stageBroker.registerElement(unregisteredComp.get());
						obj->getComponentList().registerComponent(unregisteredComp);
					}
					unregisterComps.clear();
				}
			}
		}

		void ComponentRegisterStage::runStage()
		{

		}
	}
}