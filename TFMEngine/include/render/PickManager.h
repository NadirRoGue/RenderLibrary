#ifndef __RENDERLIB_RENDER_PICKMANAGER_H__
#define __RENDERLIB_RENDER_PICKMANAGER_H__

#include "SceneObject.h"

#include "Scene.h"

namespace RenderLib
{
  namespace Render
  {
    class PickManager
    {
    private:
      SceneObject * lastPick;
      SceneObject * lastArrow;

      Scene * activeScene;

    public:
      PickManager();
      ~PickManager();

      void
      setWorkingScene(Scene * scene);
      void
      setLastPick(SceneObject * obj = nullptr);

      SceneObject *
      getLastPick();
      SceneObject *
      getLastPickArrow();

      void
      updatePicking();
    };
  } // namespace Render
} // namespace RenderLib

#endif