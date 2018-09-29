#ifndef __RENDERLIB_LOGGER_LOG_H__
#define __RENDERLIB_LOGGER_LOG_H__

#include <memory>

#include "AbstractLogger.h"

namespace RenderLib
{
	namespace Logger
	{
		class Log
		{
		private:
			static std::unique_ptr<Log> INSTANCE;
		private:
			std::unique_ptr<AbstractLogger> logger;
		public:
			Log();
			~Log();
		public:
			static Log & getInstance();
		public:
			template<class T>
			void setLogger()
			{
				if (std::is_base_of<AbstractLogger, T>::value)
				{
					logger.reset();
					logger = std::make_unique<T>();
				}
				else
				{
					logError("Log: Attempted to change logger with a non AbstractLogger derived class");
				}
			}

			void log(const std::string & text, const LogLevel & level);
			void logInfo(const std::string & text);
			void logWarning(const std::string & text);
			void logError(const std::string & text);
		};
	}
}

#endif