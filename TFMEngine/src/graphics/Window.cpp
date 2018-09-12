#include "graphics/Window.h"

#include "logger/Log.h"

#include "EngineException.h"
#include "EngineInstance.h"

#include "inputhandlers/InputManager.h"

namespace RenderLib
{
	namespace Graphics
	{
		inline void KeyboardCallbackFunc(GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			
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


		}

		void MouseInputCallbackFunc(GLFWwindow * window, int button, int action, int mods)
		{
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
		}

		void MouseMovementCallbackFunc(GLFWwindow * window, double xpos, double ypos)
		{


		}

		void ResizeCallbackFunc(GLFWwindow * window, int width, int height)
		{

		}

		// =================================================================================================

		void GLFWWindowErrorCallback(int errorCode, const char * message)
		{
			throw new EngineException(("GLFW Error (" + std::to_string(errorCode) + ": " + std::string(message)).c_str());
		}

		Window::Window(const WindowConfiguration & windowConfig)
		{
			initialize(windowConfig);
		}

		Window::~Window()
		{
			
		}

		void Window::setEngineInstance(EngineInstance * instance)
		{
			this->instance = instance;
		}

		void Window::initialize(const WindowConfiguration & windowConfig)
		{
			if (!glfwInit())
			{
				return;
			}

			//glfwWindowHint(GLFW_SAMPLES, 4);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowConfig.openGLMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowConfig.openGLMinorVersion);

			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, windowConfig.openGLContextProfile); //GLFW_OPENGL_CORE_PROFILE

			window = glfwCreateWindow
			(
				windowConfig.windowWidth,
				windowConfig.windowHeight,
				windowConfig.windowTitle.c_str(),
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

			glfwMakeContextCurrent(window);

			glfwSetWindowPos(window, windowConfig.windowPosX, windowConfig.windowPosY);

			glfwSwapInterval(2);

			// Call a default OpenGL config (may be overwritten by the passed callback)
			defaultOpenGLConfiguration();

			if (windowConfig.openGLConfigurationCallback)
			{
				windowConfig.openGLConfigurationCallback();
			}
		}

		void Window::defaultOpenGLConfiguration()
		{
			// Basic OPENGL Context configuration
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glFrontFace(GL_CCW);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
	}
}