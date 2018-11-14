#ifndef __RENDERLIB_INPUTHANDLERS_MOUSEINPUTHANDLER_H__
#define __RENDERLIB_INPUTHANDLERS_MOUSEINPUTHANDLER_H__

#include <vector>

namespace RenderLib
{
  namespace InputHandlers
  {
    enum MouseAction
    {
      MOUSE_PRESS,
      MOUSE_RELEASE
    };

    class MouseInputHandler
    {
    public:
      virtual std::vector<int>
      getUsedButtons() = 0;
      virtual void
      handleMouseClick(int button, const MouseAction & action, int x, int y)
          = 0;
    };
  } // namespace InputHandlers
} // namespace RenderLib

#endif