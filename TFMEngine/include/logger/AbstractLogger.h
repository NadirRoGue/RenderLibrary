#ifndef __RENDERLIB_LOGGER_ABSTRACTLOGGER_H__
#define __RENDERLIB_LOGGER_ABSTRACTLOGGER_H__

#include <string>

#include <chrono>
#include <ctime>

#if defined _WIN32
#define LOCALTIME(time, src) localtime_s(&time, src)
#elif defined(__unix__)
#define LOCALTIME(time, src) localtime_r(&time, src)
#else
#define LOCALTIME(time, src) &time = NULL; time = std::localtime(src)
#endif

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
			AbstractLogger();
			virtual ~AbstractLogger();
			void log(const std::string & text, const LogLevel & level);
			virtual void logInfo(const std::string & info) = 0;
			virtual void logWarning(const std::string & warn) = 0;
			virtual void logError(const std::string & error) = 0;
		};
	}
}

#endif