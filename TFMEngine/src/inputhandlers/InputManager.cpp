#include "inputhandlers/InputManager.h"

#include "logger/Log.h"

namespace RenderLib
{
  namespace InputHandlers
  {
    InputManager::InputManager() : validKBInput(false), validMInput(false)
    {
    }

    InputManager::~InputManager()
    {
      cleanInputHandlers();
    }

    void
    InputManager::registerKeyboardHandler(KeyboardHandlerPtr handler)
    {
      KeyboardInputHandler * rawHandler = handler.get();
      if (rawHandler != NULL)
      {
        for (auto & key : rawHandler->getUsedKeys())
        {
          keyboardHandlers[key].push_back(std::move(handler));
        }
      }
      else
      {
        Logger::Log::getInstance().logWarning(
            "InputManager: Attempted to register NULL KeyboardInputHandler");
      }
    }

    void
    InputManager::registerMouseHandler(MouseHandlerPtr handler)
    {
      MouseInputHandler * rawHandler = handler.get();
      if (rawHandler != NULL)
      {
        for (auto & button : rawHandler->getUsedButtons())
        {
          mouseInputHandlers[button].push_back(std::move(handler));
        }
      }
      else
      {
        Logger::Log::getInstance().logWarning(
            "InputManager: Attempted to register NULL MouseInputHandler");
      }
    }

    void
    InputManager::registerMouseMotionHandler(MouseMotionHandlerPtr handler)
    {
      if (handler.get() != NULL)
      {
        mouseMotionHandlers.push_back(std::move(handler));
      }
      else
      {
        Logger::Log::getInstance().logWarning(
            "InputManager: Attempted to register NULL MouseMotionHandler");
      }
    }

    void
    InputManager::handleKeyPress(char key, const KeyAction & action)
    {
      lastKeyPressed = key;
      lastKeyAction  = action;
      validKBInput   = true;

      std::vector<KeyboardHandlerPtr> & handlers = keyboardHandlers[key];
      if (handlers.size() > 0)
      {
        for (auto & handler : handlers)
        {
          if (handler.get() != NULL)
          {
            handler.get()->handleKeyPress(key, action);
          }
          else
          {
            Logger::Log::getInstance().logWarning(
                "InputManager: NULL keyboard input handler detected for key "
                + std::to_string(key));
          }
        }
      }
    }

    void
    InputManager::handleMouseClick(int button, const MouseAction & action,
                                   int x, int y)
    {
      lastMouseButton = button;
      lastMouseAction = action;
      lastMouseX      = x;
      lastMouseY      = y;
      validMInput     = true;

      std::vector<MouseHandlerPtr> & handlers = mouseInputHandlers[button];
      if (handlers.size() > 0)
      {
        for (auto & handler : handlers)
        {
          if (handler.get() != NULL)
          {
            handler.get()->handleMouseClick(button, action, x, y);
          }
          else
          {
            Logger::Log::getInstance().logWarning(
                "InputManager: NULL mouse input handler detected for button "
                + std::to_string(button));
          }
        }
      }
    }

    void
    InputManager::handleMouseMotion(int x, int y)
    {
      lastMouseX = x;
      lastMouseY = y;

      for (auto & handler : mouseMotionHandlers)
      {
        if (handler.get() != NULL)
        {
          handler.get()->handleMouseMotion(x, y);
        }
        else
        {
          Logger::Log::getInstance().logWarning(
              "InputManager: NULL mouse motion handler detected");
        }
      }
    }

    void
    InputManager::consumeMouseInput()
    {
      validMInput = false;
    }

    void
    InputManager::consumeKeyboardInput()
    {
      validKBInput = false;
    }

    bool
    InputManager::mouseButtonDown(int button)
    {
      return validMInput && (lastMouseButton == button)
          && (lastMouseAction == MouseAction::MOUSE_PRESS);
    }

    bool
    InputManager::mouseButtonReleased(int button)
    {
      return validMInput && (lastMouseButton != button)
          || (lastMouseAction == MouseAction::MOUSE_RELEASE);
    }

    bool
    InputManager::forceMouseButtonDown(int button)
    {
      return (lastMouseButton == button)
          && (lastMouseAction == MouseAction::MOUSE_PRESS);
    }

    bool
    InputManager::forceMouseButtonRelease(int button)
    {
      return (lastMouseButton != button)
          || (lastMouseAction == MouseAction::MOUSE_RELEASE);
    }

    bool
    InputManager::keyDown(char key)
    {
      return validKBInput && (lastKeyPressed == key)
          && (lastKeyAction == KeyAction::KEY_PRESS);
    }

    bool
    InputManager::keyPressed(char key)
    {
      return validKBInput && (lastKeyPressed == key)
          && (lastKeyAction != KeyAction::KEY_RELEASED);
    }

    bool
    InputManager::keyReleased(char key)
    {
      return validKBInput && (lastKeyPressed != key)
          || (lastKeyAction == KeyAction::KEY_RELEASED);
    }

    unsigned int
    InputManager::getLastMouseX()
    {
      return lastMouseX;
    }

    unsigned int
    InputManager::getLastMouseY()
    {
      return lastMouseY;
    }

    void
    InputManager::cleanInputHandlers()
    {
      // Clean up key handlers
      auto keyIt = keyboardHandlers.begin();
      while (keyIt != keyboardHandlers.end())
      {
        for (auto & handlers : keyIt->second)
        {
          handlers.reset();
        }
        keyIt->second.clear();
        keyIt++;
      }
      keyboardHandlers.clear();

      // Clean up mouse click handlers
      auto mouseIt = mouseInputHandlers.begin();
      while (mouseIt != mouseInputHandlers.end())
      {
        for (auto & handlers : mouseIt->second)
        {
          handlers.reset();
        }
        mouseIt->second.clear();
        mouseIt++;
      }
      mouseInputHandlers.clear();

      // Clean up mouse motion handlers
      auto mouseMotionIt = mouseMotionHandlers.begin();
      while (mouseMotionIt != mouseMotionHandlers.end())
      {
        (*mouseMotionIt).reset();
        mouseMotionIt++;
      }
      mouseMotionHandlers.clear();
    }
  } // namespace InputHandlers
} // namespace RenderLib