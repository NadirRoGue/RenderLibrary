#ifndef __RENDERLIB_SCENE_H__
#define __RENDERLIB_SCENE_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include "Camera.h"
#include "SceneObject.h"

#include "EngineException.h"

#include "inputhandlers/InputManager.h"

#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

namespace RenderLib
{
  class Scene
  {
  private:
    std::string name;

    std::vector<SceneObjectPtr> sceneObjects;

    std::unordered_map<std::string, Camera *> sceneCameras;
    std::vector<Graphics::WindowResizeObserver *> windowResizables;
    Camera * activeCamera;

    std::vector<Lighting::DirectionalLight *> direcionalLights;
    std::vector<Lighting::SpotLight *> spotLights;
    std::vector<Lighting::PointLight *> pointLights;

    InputHandlers::InputManager inputManager;

    SceneObject * sceneRoot;

    SceneObject * zArrowObj;
    SceneObject * yArrowObj;
    SceneObject * xArrowObj;

    bool sceneTreeUpdated;

  public:
    Scene(std::string name);
    ~Scene();

    const std::string &
    getSceneName();

    void
    makeCameraActive(const std::string & name);
    void
    makeCameraActive(Camera * cam);
    Camera *
    getActiveCamera();

    SceneObject *
    getSceneRoot();

    SceneObject *
    getZArrow();
    SceneObject *
    getYArrow();
    SceneObject *
    getXArrow();

    bool
    sceneTreeNeedsUpdate();
    void
    setSceneTreeNeedsUpdate(const bool & val);

    std::vector<SceneObjectPtr> &
    getSceneObjects();
    std::vector<Graphics::WindowResizeObserver *> &
    getWindowResizableObservers();
    InputHandlers::InputManager &
    getInputManager();

    template <class T>
    T *
    addObject(const std::string & objName = "")
    {
      if (std::is_base_of<SceneObject, T>::value)
      {
        std::unique_ptr<SceneObject> newObject = std::make_unique<T>();
        newObject.get()->objectName            = objName;
        T * result                             = newObject.get();
        sceneObjects.push_back(std::move(newObject));

        sceneTreeUpdated = true;

        return result;
      }

      throw EngineException(
          "Scene: Attempted to add a non-derived SceneObject element");
      return NULL;
    }

    template <class T>
    T *
    addCamera(const std::string & name)
    {
      auto alreadyExist = sceneCameras.find(name);
      if (alreadyExist != sceneCameras.end())
      {
        throw EngineException("Scene: Attempted to create a camera with a name "
                              "already present in the scene: "
                              + name);
        return NULL;
      }

      if (std::is_base_of<Camera, T>::value)
      {
        std::unique_ptr<SceneObject> newCamPtr = std::make_unique<T>();

        T * result = static_cast<T *>(newCamPtr.get());

        if (activeCamera == NULL)
        {
          activeCamera = result;
        }

        newCamPtr.get()->objectName = name;

        sceneObjects.push_back(std::move(newCamPtr));
        sceneCameras[name] = result;
        windowResizables.push_back(
            static_cast<Graphics::WindowResizeObserver *>(result));

        sceneTreeUpdated = true;

        return result;
      }

      throw EngineException(
          "Scene: Attempted to add a non-derived Camera camera");
      return NULL;
    }

    Lighting::DirectionalLight *
    addDirectionalLight(const std::string & name);
    Lighting::PointLight *
    addPointLight(const std::string & name);
    Lighting::SpotLight *
    addSpotLight(const std::string & name);

    std::vector<Lighting::DirectionalLight *> &
    getDirectionalLights();
    std::vector<Lighting::PointLight *> &
    getPointLights();
    std::vector<Lighting::SpotLight *> &
    getSpotLights();
  };
} // namespace RenderLib

#endif