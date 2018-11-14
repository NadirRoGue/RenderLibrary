#ifndef __RENDERLIB_DEFAULTIMPL_PICKARROWMOVEMENT_H__
#define __RENDERLIB_DEFAULTIMPL_PICKARROWMOVEMENT_H__

#include "defaultimpl/components/UserScript.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    enum class ArrowAxis
    {
      AXIS_FORWARD,
      AXIS_RIGHT,
      AXIS_UP
    };

    class PickArrowMovement : public UserScript
    {
    private:
      int lastMouseX, lastMouseY;

    public:
      ArrowAxis arrowType;

    public:
      void
      start();
      void
      update();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif