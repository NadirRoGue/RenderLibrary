#include "graphics/defaultwindowhandlers/GLFWWindowHandler.h"

#include "EngineInstance.h"
#include "EngineException.h"

#include "inputhandlers/InputManager.h"

#include <iostream>

#include "Camera.h"

namespace RenderLib
{
	namespace Graphics
	{
		inline void KeyboardCallbackFunc(GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			GLFWWindowHandler * handler = reinterpret_cast<GLFWWindowHandler*>(glfwGetWindowUserPointer(window));

			InputHandlers::KeyAction actionEnum;
			switch (action)
			{
			case GLFW_PRESS:
				actionEnum = InputHandlers::KeyAction::KEY_PRESS;
				break;
			case GLFW_RELEASE:
				actionEnum = InputHandlers::KeyAction::KEY_RELEASED;
				break;
			case GLFW_REPEAT:
				actionEnum = InputHandlers::KeyAction::KEY_REPEAT;
				break;
			default:
				return;
			}

			if (!handler->instance)
			{
				//throw EngineException(("GLFWWindowHandler: NULL EngineInstance for window " + handler->getTitle()).c_str());
				return;
			}

			EngineInstance * instance = handler->instance;
			Scene * activeScene = instance->getSceneManager().getActiveScene();
			if (activeScene)
			{
				activeScene->getInputManager().handleKeyPress((char)key, actionEnum);
			}
		}

		void MouseInputCallbackFunc(GLFWwindow * window, int button, int action, int mods)
		{
			GLFWWindowHandler * handler = reinterpret_cast<GLFWWindowHandler*>(glfwGetWindowUserPointer(window));

			InputHandlers::MouseAction actionEnum;
			switch (action)
			{
			case GLFW_PRESS:
				actionEnum = InputHandlers::MouseAction::MOUSE_PRESS;
				break;
			case GLFW_RELEASE:
				actionEnum = InputHandlers::MouseAction::MOUSE_RELEASE;
				break;
			default:
				return;
			}

			if (!handler->instance)
			{
				//throw EngineException(("GLFWWindowHandler: NULL EngineInstance for window " + handler->getTitle()).c_str());
				return;
			}

			EngineInstance * instance = handler->instance;
			Scene * activeScene = instance->getSceneManager().getActiveScene();
			if (activeScene)
			{
				activeScene->getInputManager().handleMouseClick(button, actionEnum, (int)(handler->lastMouseX), int(handler->lastMouseY));
			}
		}

		void MouseMovementCallbackFunc(GLFWwindow * window, double xpos, double ypos)
		{
			GLFWWindowHandler * handler = reinterpret_cast<GLFWWindowHandler*>(glfwGetWindowUserPointer(window));
			if (!handler->instance)
			{
				//throw EngineException(("GLFWWindowHandler: NULL EngineInstance for window " + handler->getTitle()).c_str());
				return;
			}

			handler->lastMouseX = xpos;
			handler->lastMouseY = ypos;

			EngineInstance * instance = handler->instance;
			Scene * activeScene = instance->getSceneManager().getActiveScene();
			if (activeScene)
			{
				activeScene->getInputManager().handleMouseMotion((int)(handler->lastMouseX), int(handler->lastMouseY));
			}
		}

		void ResizeCallbackFunc(GLFWwindow * window, int width, int height)
		{
			GLFWWindowHandler * handler = reinterpret_cast<GLFWWindowHandler*>(glfwGetWindowUserPointer(window));
			if (!handler->instance)
			{
				//throw EngineException(("GLFWWindowHandler: NULL EngineInstance for window " + handler->getTitle()).c_str());
				return;
			}

			// TODO: Replace direct camera access to resizable window observable, to allow include
			// different objects than just cameras
			EngineInstance * instance = handler->instance;
			Scene * activeScene = instance->getSceneManager().getActiveScene();
			if (activeScene)
			{
				for (auto & cam : activeScene->getAllCameras())
				{
					cam.get()->onWindowResize(width, height);
				}
			}
		}

		// =================================================================================================

		void GLFWWindowErrorCallback(int errorCode, const char * message)
		{
			throw new EngineException(("GLFW Error (" + std::to_string(errorCode) + ": " + std::string(message)).c_str());
		}

		// =================================================================================================

		void GLFWWindowFocusedCallback(GLFWwindow * window, int focused)
		{

		}

		// =================================================================================================

		GLFWWindowHandler::GLFWWindowHandler(WindowConfiguration config)
			: WindowHandler(config)
			, lastMouseX(0.0)
			, lastMouseY(0.0)

		{
		}

		GLFWWindowHandler::~GLFWWindowHandler()
		{
			if (window)
			{
				glfwDestroyWindow(window);
			}
		}

		void GLFWWindowHandler::initialize()
		{
			if (!glfwInit())
			{
				return;
			}

			//glfwWindowHint(GLFW_SAMPLES, 4);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.openGLMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.openGLMinorVersion);

			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, config.openGLContextProfile); //GLFW_OPENGL_CORE_PROFILE

			window = glfwCreateWindow
			(
				config.windowWidth,
				config.windowHeight,
				config.windowTitle.c_str(),
				NULL,
				NULL
			);

			if (!window)
			{
				glfwTerminate();
				throw EngineException("Window: Could not create GLFW window");
			}

			glfwSetKeyCallback(window, KeyboardCallbackFunc);
			glfwSetCursorPosCallback(window, MouseMovementCallbackFunc);
			glfwSetMouseButtonCallback(window, MouseInputCallbackFunc);
			glfwSetFramebufferSizeCallback(window, ResizeCallbackFunc);
			glfwSetWindowFocusCallback(window, GLFWWindowFocusedCallback);
			glfwSetErrorCallback(GLFWWindowErrorCallback);

			glfwSetWindowUserPointer(window, this);

			glfwMakeContextCurrent(window);

			glfwSetWindowPos(window, config.windowPosX, config.windowPosY);

			glfwSwapInterval(2);

			// Call a default OpenGL config (may be overwritten by the passed callback)
			defaultOpenGLConfiguration();

			if (config.openGLConfigurationCallback)
			{
				config.openGLConfigurationCallback();
			}

			if (glewIsSupported("ARB_direct_state_access"))
			{
				std::cout << "is supported" << std::endl;
			}
		}

		bool GLFWWindowHandler::isActive()
		{
			return !glfwWindowShouldClose(window);
		}

		void GLFWWindowHandler::onRenderLoopIteration()
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		void GLFWWindowHandler::cleanUp()
		{
			glfwDestroyWindow(window);
			glfwTerminate();
		}
	}
}