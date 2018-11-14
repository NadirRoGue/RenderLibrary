#include "defaultimpl/pipelinestages/ComponentRegisterStage.h"

#include "pipeline/PipelineManager.h"

#include "graphics/WindowHandler.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    void
    ComponentRegisterStage::preRunStage()
    {
      initAndRegister();
    }

    void
    ComponentRegisterStage::runStage()
    {
      initAndRegister();
    }

    void
    ComponentRegisterStage::initAndRegister()
    {
      Pipeline::PipelineBroker & stageBroker
          = engineInstance->getPipelineManager().getStageBroker();
      Scene * scene = engineInstance->getSceneManager().getActiveScene();

      std::vector<SceneObjectPtr> & objects = scene->getSceneObjects();
      for (auto & objPtr : objects)
      {
        SceneObject * obj = objPtr.get();

        if (!obj->initialized)
        {
          obj->initialize();
        }

        if (obj->getParent() == NULL && obj != scene->getSceneRoot())
        {
          obj->setParent(scene->getSceneRoot());
        }

        std::vector<std::unique_ptr<Component>> & unregisterComps
            = obj->getComponentList().getUnregisteredComponents();
        if (unregisterComps.size() > 0)
        {
          for (auto & unregisteredComp : unregisterComps)
          {
            unregisteredComp.get()->engineInstance = engineInstance;
            unregisteredComp.get()->start();

            stageBroker.registerElement(unregisteredComp.get());
            obj->getComponentList().registerComponent(unregisteredComp);
          }
          unregisterComps.clear();
        }
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib