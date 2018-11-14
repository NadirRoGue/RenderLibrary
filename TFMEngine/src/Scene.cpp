#include "Scene.h"

#include "logger/Log.h"

#include "CPU/mesh/MeshManager.h"
#include "defaultimpl/components/MeshFilter.h"
#include "defaultimpl/components/MeshRenderer.h"
#include "defaultimpl/components/PickArrowMovement.h"

#include "defaultimpl/shaders/PickArrowProgram.h"

namespace RenderLib
{
  Scene::Scene(std::string sceneName)
    : name(sceneName), activeCamera(NULL), sceneTreeUpdated(false)
  {
    sceneRoot             = addObject<SceneObject>();
    sceneRoot->objectName = "Root";

    zArrowObj = addObject<SceneObject>();
    zArrowObj->addComponent<DefaultImpl::MeshFilter>()->mesh
        = CPU::Mesh::MeshManager::getInstance().getZArrowMesh();
    DefaultImpl::MeshRenderer * zRend
        = zArrowObj->addComponent<DefaultImpl::MeshRenderer>();
    zRend->enabled = false;
    zRend->setShaderType<DefaultImpl::PickArrowProgram>();
    zRend->preferredRender = DefaultImpl::FORWARD_RENDER;
    zArrowObj->addComponent<DefaultImpl::PickArrowMovement>()->arrowType
        = DefaultImpl::ArrowAxis::AXIS_FORWARD;

    yArrowObj = addObject<SceneObject>();
    yArrowObj->addComponent<DefaultImpl::MeshFilter>()->mesh
        = CPU::Mesh::MeshManager::getInstance().getYArrowMesh();
    DefaultImpl::MeshRenderer * yRend
        = yArrowObj->addComponent<DefaultImpl::MeshRenderer>();
    yRend->enabled = false;
    yRend->setShaderType<DefaultImpl::PickArrowProgram>();
    yRend->preferredRender = DefaultImpl::FORWARD_RENDER;
    yArrowObj->addComponent<DefaultImpl::PickArrowMovement>()->arrowType
        = DefaultImpl::ArrowAxis::AXIS_UP;

    xArrowObj = addObject<SceneObject>();
    xArrowObj->addComponent<DefaultImpl::MeshFilter>()->mesh
        = CPU::Mesh::MeshManager::getInstance().getXArrowMesh();
    DefaultImpl::MeshRenderer * xRend
        = xArrowObj->addComponent<DefaultImpl::MeshRenderer>();
    xRend->enabled = false;
    xRend->setShaderType<DefaultImpl::PickArrowProgram>();
    xRend->preferredRender = DefaultImpl::FORWARD_RENDER;
    xArrowObj->addComponent<DefaultImpl::PickArrowMovement>()->arrowType
        = DefaultImpl::ArrowAxis::AXIS_RIGHT;
  }

  Scene::~Scene()
  {
  }

  const std::string &
  Scene::getSceneName()
  {
    return name;
  }

  void
  Scene::makeCameraActive(const std::string & name)
  {
    auto it = sceneCameras.find(name);
    if (it != sceneCameras.end())
    {
      activeCamera = it->second;
    }
    else
    {
      Logger::Log::getInstance().logWarning(
          "Scene: Attempt to set camera " + name
          + " as active failed: No such camera");
    }
  }

  void
  Scene::makeCameraActive(Camera * cam)
  {
    activeCamera = cam;
  }

  Camera *
  Scene::getActiveCamera()
  {
    return activeCamera;
  }

  SceneObject *
  Scene::getSceneRoot()
  {
    return sceneRoot;
  }

  SceneObject *
  Scene::getZArrow()
  {
    return zArrowObj;
  }

  SceneObject *
  Scene::getYArrow()
  {
    return yArrowObj;
  }

  SceneObject *
  Scene::getXArrow()
  {
    return xArrowObj;
  }

  bool
  Scene::sceneTreeNeedsUpdate()
  {
    return sceneTreeUpdated;
  }

  void
  Scene::setSceneTreeNeedsUpdate(const bool & val)
  {
    sceneTreeUpdated = val;
  }

  std::vector<SceneObjectPtr> &
  Scene::getSceneObjects()
  {
    return sceneObjects;
  }

  std::vector<Graphics::WindowResizeObserver *> &
  Scene::getWindowResizableObservers()
  {
    return windowResizables;
  }

  InputHandlers::InputManager &
  Scene::getInputManager()
  {
    return inputManager;
  }

  Lighting::DirectionalLight *
  Scene::addDirectionalLight(const std::string & name)
  {
    std::unique_ptr<SceneObject> newDL
        = std::make_unique<Lighting::DirectionalLight>();
    Lighting::DirectionalLight * result
        = static_cast<Lighting::DirectionalLight *>(newDL.get());

    newDL.get()->objectName = name;

    sceneTreeUpdated = true;

    direcionalLights.push_back(result);
    sceneObjects.push_back(std::move(newDL));

    return result;
  }

  Lighting::SpotLight *
  Scene::addSpotLight(const std::string & name)
  {
    std::unique_ptr<SceneObject> newSL
        = std::make_unique<Lighting::SpotLight>();
    Lighting::SpotLight * result
        = static_cast<Lighting::SpotLight *>(newSL.get());

    newSL.get()->objectName = name;

    sceneTreeUpdated = true;

    spotLights.push_back(result);
    sceneObjects.push_back(std::move(newSL));

    return result;
  }

  Lighting::PointLight *
  Scene::addPointLight(const std::string & name)
  {
    std::unique_ptr<SceneObject> newPL
        = std::make_unique<Lighting::PointLight>();
    Lighting::PointLight * result
        = static_cast<Lighting::PointLight *>(newPL.get());

    newPL.get()->objectName = name;

    sceneTreeUpdated = true;

    pointLights.push_back(result);
    sceneObjects.push_back(std::move(newPL));

    return result;
  }

  std::vector<Lighting::DirectionalLight *> &
  Scene::getDirectionalLights()
  {
    return direcionalLights;
  }

  std::vector<Lighting::SpotLight *> &
  Scene::getSpotLights()
  {
    return spotLights;
  }

  std::vector<Lighting::PointLight *> &
  Scene::getPointLights()
  {
    return pointLights;
  }
} // namespace RenderLib