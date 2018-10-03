#ifndef __RENDERLIB_DEFAULTIMPL_GLFWWINDOW_H__
#define __RENDERLIB_DEFAULTIMPL_GLFWWINDOW_H__

#include "graphics/WindowHandler.h"

#include <GLFW/glfw3.h>

namespace RenderLib
{
	namespace DefaultImpl
	{
		class GLFWWindowHandler : public Graphics::WindowHandler
		{
		private:
			GLFWwindow * window;

		public:
			double lastMouseX;
			double lastMouseY;

		public:
			GLFWWindowHandler(Graphics::WindowConfiguration config);
			~GLFWWindowHandler();

			virtual void initializeWindowContext();

			bool isActive();

			void activateContext();

			void onRenderLoopIteration();

			double elapsedTimeSinceStart();

			void cleanUp();
		};
	}
}

#endif