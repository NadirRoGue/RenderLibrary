#ifndef __RENDERLIB_DEFAULTIMPL_CAMERACONTROLLER_H__
#define __RENDERLIB_DEFAULTIMPL_CAMERACONTROLLER_H__

#include "defaultimpl/components/UserScript.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class CameraController : public UserScript
    {
    private:
      float moveSpeed;
      unsigned int lastMouseX, lastMouseY;

    public:
      CameraController();
      void
      update();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
