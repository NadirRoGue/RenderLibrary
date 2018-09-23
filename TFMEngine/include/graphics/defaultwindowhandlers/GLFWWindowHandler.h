#ifndef __RENDERLIB_GRAPHICS_GLFWWINDOW_H__
#define __RENDERLIB_GRAPHICS_GLFWWINDOW_H__

#include "graphics/WindowHandler.h"

#include <GLFW/glfw3.h>

namespace RenderLib
{
	namespace Graphics
	{
		class GLFWWindowHandler : public WindowHandler
		{
		private:
			GLFWwindow * window;

		public:
			double lastMouseX;
			double lastMouseY;

		public:
			GLFWWindowHandler(WindowConfiguration config);
			~GLFWWindowHandler();

			virtual void initializeWindowContext();

			bool isActive();

			void activateContext();

			void onRenderLoopIteration();

			void cleanUp();
		};
	}
}

#endif