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
			void setLogger(std::unique_ptr<AbstractLogger> && newLogger);
			void log(const std::string & text, const LogLevel & level);
			void logInfo(const std::string & text);
			void logWarning(const std::string & text);
			void logError(const std::string & text);
		};
	}
}

#endif