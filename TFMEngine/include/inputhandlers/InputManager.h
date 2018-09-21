#ifndef __RENDERLIB_INPUTHANDLERS_INPUTMANAGER_H__
#define __RENDERLIB_INPUTHANDLERS_INPUTMANAGER_H__

#include <map>
#include <vector>
#include <memory>

#include "inputhandlers/KeyboardInputHandler.h"
#include "inputhandlers/MouseInputHandler.h"
#include "inputhandlers/MouseMotionHandler.h"

namespace RenderLib
{
	namespace InputHandlers
	{
		typedef std::unique_ptr<KeyboardInputHandler> KeyboardHandlerPtr;
		typedef std::unique_ptr<MouseInputHandler> MouseHandlerPtr;
		typedef std::unique_ptr<MouseMotionHandler> MouseMotionHandlerPtr;

		class InputManager;

		class InputManager
		{
		private:
			std::map<char, std::vector<KeyboardHandlerPtr>> keyboardHandlers;
			std::map<int, std::vector<MouseHandlerPtr>> mouseInputHandlers;
			std::vector<MouseMotionHandlerPtr> mouseMotionHandlers;

			char lastKeyPressed;
			KeyAction lastKeyAction;

			int lastMouseButton;
			MouseAction lastMouseAction;

		public:
			InputManager();
			~InputManager();

			void registerKeyboardHandler(KeyboardHandlerPtr handler);
			void registerMouseHandler(MouseHandlerPtr handler);
			void registerMouseMotionHandler(MouseMotionHandlerPtr handler);

			void handleKeyPress(char key, const KeyAction & action);
			void handleMouseClick(int button, const MouseAction & action, int x, int y);
			void handleMouseMotion(int x, int y);

			bool mouseButtonDown(int button);
			bool mouseButtonReleased(int button);
			bool keyDown(char key);
			bool keyPressed(char key);
			bool keyReleased(char key);

			void cleanInputHandlers();
		};
	}
}

#endif