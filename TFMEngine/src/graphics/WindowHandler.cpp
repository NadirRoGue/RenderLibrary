#include "graphics/WindowHandler.h"

#include "logger/Log.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace Graphics
	{
		WindowHandler::WindowHandler(WindowConfiguration windowConfig)
			: config(windowConfig)
		{
		}

		WindowHandler::~WindowHandler()
		{
			
		}

		const std::string & WindowHandler::getTitle()
		{
			return config.windowTitle;
		}

		void WindowHandler::initialize()
		{
			initializeWindowContext();

			if (glewInit() != GLEW_OK)
			{
				throw EngineException("WindowHandler: Error while initializing GLEW");
			}

			// Call a default OpenGL config (may be overwritten by the passed callback)
			defaultOpenGLConfiguration();

			if (config.openGLConfigurationCallback)
			{
				config.openGLConfigurationCallback();
			}

			const GLubyte *oglVersion = glGetString(GL_VERSION);
		  Logger::Log::getInstance().logInfo("This system supports OpenGL Version: " + std::string((const char*)oglVersion));
		}

		void WindowHandler::defaultOpenGLConfiguration()
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