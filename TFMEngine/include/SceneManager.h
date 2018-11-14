#ifndef __RENDERLIB_SCENEMANAGER_H__
#define __RENDERLIB_SCENEMANAGER_H__

#include <memory>
#include <unordered_map>

#include "Scene.h"

namespace RenderLib
{
  class SceneManager
  {
  private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> sceneCache;

    Scene * activeScene;

  public:
    SceneManager();
    ~SceneManager();

    Scene *
    createScene(const std::string & sceneName);

    void
    deleteScene(const std::string & sceneName);

    Scene *
    getScene(const std::string & sceneName);
    Scene *
    getActiveScene();
    void
    setActiveScene(std::string sceneName);
  };
} // namespace RenderLib

#endif