#include "EngineInstance.h"

#include "pipeline/PipelineManager.h"

#include "graphics/ContextManager.h"

#include "EngineException.h"
#include "SceneManager.h"

namespace RenderLib
{
  EngineInstance::EngineInstance(const unsigned int & ID,
                                 const std::string & instanceName,
                                 Graphics::WindowHandler * handler)
    : instanceID(ID)
    , instanceName(instanceName)
    , window(handler)
    , enableFlag(true)
  {
    if (!handler)
    {
      throw EngineException(("EngineInstance: Failed constructing instance "
                             + instanceName + ", WindowHandler is NULL")
                                .c_str());
    }

    pipelineManager.setEngineInstance(this);
    fboManager.setEngineInstance(this);
    window->instance = this;
  }

  EngineInstance::~EngineInstance()
  {
  }

  // ==============================================================
  // ACCESORS

  const unsigned int &
  EngineInstance::getInstanceID()
  {
    return instanceID;
  }

  const std::string &
  EngineInstance::getInstanceName()
  {
    return instanceName;
  }

  Graphics::WindowHandler *
  EngineInstance::getWindow()
  {
    return window;
  }

  GPU::Mesh::GPUMeshManager &
  EngineInstance::getGPUMeshManager()
  {
    return gpuMeshManager;
  }

  GPU::Texture::GPUTextureManager &
  EngineInstance::getGPUTextureManager()
  {
    return gpuTextureManager;
  }

  Time &
  EngineInstance::getTime()
  {
    return timeHandler;
  }

  void
  EngineInstance::disable()
  {
    enableFlag = false;
  }

  bool
  EngineInstance::isEnabled()
  {
    return enableFlag;
  }

  Pipeline::PipelineManager &
  EngineInstance::getPipelineManager()
  {
    return pipelineManager;
  }

  SceneManager &
  EngineInstance::getSceneManager()
  {
    return sceneManager;
  }

  Render::FBOManager &
  EngineInstance::getFBOManager()
  {
    return fboManager;
  }

  Render::PickManager &
  EngineInstance::getPickManager()
  {
    return pickManager;
  }

  GPU::Program::ProgramManager &
  EngineInstance::getProgramManager()
  {
    return gpuProgramManager;
  }

  GPU::Light::LightManager &
  EngineInstance::getGPULightManager()
  {
    return lightManager;
  }

  // ==============================================================
  // EXECUTION

  void
  EngineInstance::loadActiveScene()
  {
    Scene * scene = sceneManager.getActiveScene();
    pipelineManager.initializeStages();
  }

  void
  EngineInstance::loadScene(const std::string & sceneName)
  {
    Scene * scene = sceneManager.getScene(sceneName);
    if (scene)
    {
      sceneManager.setActiveScene(sceneName);
      loadActiveScene();
    }
    else
    {
      std::string message = "EngineInstance [name=" + instanceName + ", ID="
          + std::to_string(instanceID) + "]: Failed to load scene " + sceneName;
      throw EngineException(message.c_str());
    }
  }

  void
  EngineInstance::executeIteration()
  {
    // Execute pipeline stages
    pipelineManager.executePipeline();

    // Swap buffers
    acquireContext();
    window->onRenderLoopIteration();
    releaseContext();
  }

  void
  EngineInstance::cleanUp()
  {
    pipelineManager.finishStages();
    window->cleanUp();
  }

  // ==============================================================
  // CONTEXT MANAGEMENT

  void
  EngineInstance::acquireContext()
  {
    Graphics::ContextManager().getInstance().aquireContext();
    window->activateContext();
  }

  void
  EngineInstance::releaseContext()
  {
    Graphics::ContextManager::getInstance().releaseContext();
  }
} // namespace RenderLib