#ifndef __RENDERLIB_GRAPHICS_CONTEXTMANAGER_H__
#define __RENDERLIB_GRAPHICS_CONTEXTMANAGER_H__

#include <memory>

#include <mutex>
#include <condition_variable>

#include <vector>

#include <string>

#include "EngineInstance.h"
#include "EngineException.h"

#include "graphics/WindowHandler.h"

namespace RenderLib
{
	namespace Graphics
	{
		class ContextManager
		{
		private:
			static std::unique_ptr<ContextManager> INSTANCE;
		private:
			std::mutex mutex;
			std::condition_variable monitor;

			std::vector<std::unique_ptr<WindowHandler>> windows;

			bool contextTaken;
		public:
			static ContextManager & getInstance();
		public:
			ContextManager();
			~ContextManager();

			void aquireContext();
			void releaseContext();

			template<class T>
			T * createWindow(const WindowConfiguration & config)
			{
				if (!std::is_base_of<WindowHandler, T>::value)
				{
					std::string message = "ContextManager: Attempted to create window with a non WindowHandler derived class (" + std::string(typeid(T).name()) + ")";
					throw EngineException(message.c_str());
				}

				std::unique_ptr<T> window = std::make_unique<T>(config);
				T * result = window.get();
				windows.push_back(std::move(window));

				return result;
			}
		};
	}
}

#endif