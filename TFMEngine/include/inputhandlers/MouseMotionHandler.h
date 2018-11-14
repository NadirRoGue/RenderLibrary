#ifndef __RENDERLIB_INPUTHANDLERS_MOUSEMOTIONHANDLER_H__
#define __RENDERLIB_INPUTHANDLERS_MOUSEMOTIONHANDLER_H__

namespace RenderLib
{
  namespace InputHandlers
  {
    class MouseMotionHandler
    {
    public:
      virtual void
      handleMouseMotion(int x, int y)
          = 0;
    };
  } // namespace InputHandlers
} // namespace RenderLib

#endif