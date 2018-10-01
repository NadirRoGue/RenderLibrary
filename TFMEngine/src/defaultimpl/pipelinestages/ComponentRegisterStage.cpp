#include "defaultimpl/pipelinestages/ComponentRegisterStage.h"

#include "pipeline/PipelineManager.h"

#include "graphics/WindowHandler.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void ComponentRegisterStage::preRunStage()
		{
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			Graphics::WindowHandler * window = engineInstance->getWindow();
			for (auto cam : scene->getWindowResizableObservers())
			{
				cam->setWindowSize(window->getWidth(), window->getHeight());
			}

			initAndRegister();
		}

		void ComponentRegisterStage::runStage()
		{
			initAndRegister();
		}

		void ComponentRegisterStage::initAndRegister()
		{
			Pipeline::PipelineBroker & stageBroker = engineInstance->getPipelineManager().getStageBroker();
			Scene * scene = engineInstance->getSceneManager().getActiveScene();

			std::vector<SceneObjectPtr> & objects = scene->getSceneObjects();
			for (auto & objPtr : objects)
			{
				SceneObject * obj = objPtr.get();

				if (!obj->initialized)
				{
					obj->initialize();
				}

				if (obj->getParent() == NULL)
				{
					obj->setParent(scene->getSceneRoot());
				}

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
	}
}