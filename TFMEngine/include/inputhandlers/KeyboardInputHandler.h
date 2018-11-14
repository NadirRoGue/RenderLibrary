#ifndef __RENDERLIB_INPUTHANDLERS_KEYBOARDINPUTHANDLER_H__
#define __RENDERLIB_INPUTHANDLERS_KEYBOARDINPUTHANDLER_H__

#include <vector>

namespace RenderLib
{
  namespace InputHandlers
  {
    enum KeyAction
    {
      KEY_PRESS,
      KEY_REPEAT,
      KEY_RELEASED
    };

    class KeyboardInputHandler
    {
    public:
      virtual std::vector<char>
      getUsedKeys() = 0;
      virtual void
      handleKeyPress(char key, const KeyAction & action);
    };
  } // namespace InputHandlers
} // namespace RenderLib

#endif