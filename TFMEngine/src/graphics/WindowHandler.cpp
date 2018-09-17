#include "graphics/WindowHandler.h"

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