#ifndef __RENDERLIB_LOGGER_CONSOLELOGGER_H__
#define __RENDERLIB_LOGGER_CONSOLELOGGER_H__

#include "logger/AbstractLogger.h"

namespace RenderLib
{
	namespace Logger
	{
		class ConsoleLogger : public AbstractLogger
		{
		public:
			void logInfo(const std::string & info);
			void logWarning(const std::string & warn);
			void logError(const std::string & error);
		};
	}
}

#endif