#ifndef __RENDERLIB_GRAPHICS_WINDOW_H__
#define __RENDERLIB_GRAPHICS_WINDOW_H__

#include <string>
#include <functional>

#include <GL/glew.h>

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

		class WindowHandler
		{
		public:
			EngineInstance * instance;
		protected:
			WindowConfiguration config;
		public:
			WindowHandler(WindowConfiguration windowConfig);
			~WindowHandler();

			unsigned int getWidth();
			unsigned int getHeight();

			const std::string & getTitle();

			virtual void initialize();

			virtual void initializeWindowContext() = 0;

			virtual bool isActive() = 0;

			virtual void activateContext() = 0;

			virtual void onRenderLoopIteration() = 0;

			virtual void cleanUp() = 0;

			virtual double elapsedTimeSinceStart() = 0;

		protected:
			void defaultOpenGLConfiguration();
		};
	}
}

#endif