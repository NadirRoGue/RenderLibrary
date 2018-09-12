#include "logger/ConsoleLogger.h"

#include <iostream>

#ifdef _WIN32
#define PRINTF fprintf_s
#else
#define PRINTF fprintf
#endif

namespace RenderLib
{
	namespace Logger
	{
		void ConsoleLogger::logInfo(const std::string & message)
		{
			PRINTF(stdout, "%s\n", message.c_str());
		}

		void ConsoleLogger::logWarning(const std::string & warning)
		{
			PRINTF(stdout, "%s\n", warning.c_str());
		}

		void ConsoleLogger::logError(const std::string & error)
		{
			PRINTF(stderr, "%s\n", error.c_str());
		}
	}
}