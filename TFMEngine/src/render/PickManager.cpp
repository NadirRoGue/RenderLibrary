#include "render/PickManager.h"

#include "logger/Log.h"

#include "defaultimpl/components/MeshRenderer.h"

namespace RenderLib
{
  namespace Render
  {
    PickManager::PickManager()
      : lastPick(nullptr), lastArrow(nullptr), activeScene(nullptr)
    {
    }

    PickManager::~PickManager()
    {
    }

    void
    PickManager::setWorkingScene(Scene * scene)
    {
      activeScene = scene;
    }

    void
    PickManager::setLastPick(SceneObject * obj)
    {
      if (activeScene == nullptr)
      {
        return;
      }

      SceneObject * zArrow = activeScene->getZArrow();
      SceneObject * yArrow = activeScene->getYArrow();
      SceneObject * xArrow = activeScene->getXArrow();

      if (obj == zArrow || obj == yArrow || obj == xArrow)
      {
        activeScene->getInputManager().consumeMouseInput();
        lastArrow = obj;
        return;
      }

      if (obj != nullptr)
      {
        activeScene->getInputManager().consumeMouseInput();
        lastPick                                                   = obj;
        zArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = true;
        yArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = true;
        xArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = true;
      }
      else
      {
        if (lastArrow != nullptr)
        {
          lastArrow = nullptr;
          activeScene->getInputManager().consumeMouseInput();
        }
        else if (lastPick != nullptr)
        {
          zArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = false;
          yArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = false;
          xArrow->getComponent<DefaultImpl::MeshRenderer>()->enabled = false;
          lastPick                                                   = nullptr;
          activeScene->getInputManager().consumeMouseInput();
        }
      }
    }

    void
    PickManager::updatePicking()
    {
      if (activeScene != nullptr && lastPick != nullptr)
      {
        SceneObject * zArrow = activeScene->getZArrow();
        SceneObject * yArrow = activeScene->getYArrow();
        SceneObject * xArrow = activeScene->getXArrow();

        Transform & transform = lastPick->transform;

        zArrow->transform = transform;
        zArrow->transform.setScale(VECTOR3(1, 1, 1));
        yArrow->transform = transform;
        yArrow->transform.setScale(VECTOR3(1, 1, 1));
        xArrow->transform = transform;
        xArrow->transform.setScale(VECTOR3(1, 1, 1));
      }
    }

    SceneObject *
    PickManager::getLastPick()
    {
      return lastPick;
    }

    SceneObject *
    PickManager::getLastPickArrow()
    {
      return lastArrow;
    }

  } // namespace Render
} // namespace RenderLib