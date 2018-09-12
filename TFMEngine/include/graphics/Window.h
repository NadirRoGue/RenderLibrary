#ifndef __RENDERLIB_GRAPHICS_WINDOW_H__
#define __RENDERLIB_GRAPHICS_WINDOW_H__

#include <string>
#include <functional>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

namespace RenderLib
{
	class EngineInstance;

	namespace Graphics
	{
		typedef struct WindowConfiguration
		{
			unsigned int windowPosX;
			unsigned int windowPosY;

			unsigned int windowWidth;
			unsigned int windowHeight;

			unsigned int openGLMajorVersion;
			unsigned int openGLMinorVersion;

			unsigned int openGLContextProfile;

			std::string windowTitle;

			std::function<void(void)> openGLConfigurationCallback;

		} WindowConfiguration;

		class Window
		{
		private:
			EngineInstance * instance;
			GLFWwindow * window;
		public:
			Window(const WindowConfiguration & windowConfig);
			~Window();

			void setEngineInstance(EngineInstance * instance);

			void initialize(const WindowConfiguration & winConfig);
		private:
			void defaultOpenGLConfiguration();
		};
	}
}

#endif