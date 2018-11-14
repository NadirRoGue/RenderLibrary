#include "defaultimpl/pipelinestages/IterationEndStage.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    void
    IterationEndStage::preRunStage()
    {
      // Make sure we resize all cameras/FBOs to window size
      // After they have been initialized during the preRunStages calls
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      Graphics::WindowHandler * window = engineInstance->getWindow();
      for (auto cam : scene->getWindowResizableObservers())
      {
        cam->setWindowSize(window->getWidth(), window->getHeight());
      }

      engineInstance->getFBOManager().onResize(window->getWidth(),
                                               window->getHeight());
    }

    void
    IterationEndStage::runStage()
    {
      // Swap upload/render buffer for dynamic meshes
      engineInstance->getGPUMeshManager().swapDynamicBuffers();

      engineInstance->getPickManager().updatePicking();

      // Update time
      engineInstance->acquireContext();

      double elapsedTime = engineInstance->getWindow()->elapsedTimeSinceStart();
      engineInstance->getTime().update(elapsedTime);
      engineInstance->releaseContext();
    }

    void
    IterationEndStage::postRunStage()
    {
      engineInstance->acquireContext();

      // Clear programs created on this context
      engineInstance->getProgramManager().clear();

      engineInstance->releaseContext();

      // Non context-related clean ups
      engineInstance->getPipelineManager().getThreadPool().shutDown();

      // Call component's destroy method
      for (auto & obj : engineInstance->getSceneManager()
                            .getActiveScene()
                            ->getSceneObjects())
      {
        SceneObject * objPtr = obj.get();
        for (auto & comp : objPtr->getAllComponents())
        {
          comp.get()->destroy();
        }
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib