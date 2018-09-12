#ifndef __RENDERLIB_LOGGER_ABSTRACTLOGGER_H__
#define __RENDERLIB_LOGGER_ABSTRACTLOGGER_H__

#include <string>

namespace RenderLib
{
	namespace Logger
	{
		enum LogLevel
		{
			LEVEL_INFO,
			LEVEL_WARNING,
			LEVEL_ERROR
		};

		class AbstractLogger
		{
		public:
			void log(const std::string & text, const LogLevel & level);
			virtual void logInfo(const std::string & info) = 0;
			virtual void logWarning(const std::string & warn) = 0;
			virtual void logError(const std::string & error) = 0;
		};
	}
}

#endif