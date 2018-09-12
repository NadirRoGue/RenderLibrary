#include "logger/AbstractLogger.h"

namespace RenderLib
{
	namespace Logger
	{
		void AbstractLogger::log(const std::string & text, const LogLevel & level)
		{
			switch (level)
			{
			case LogLevel::LEVEL_INFO:
				logInfo(text);
				break;
			case LogLevel::LEVEL_WARNING:
				logWarning(text);
				break;
			case LogLevel::LEVEL_ERROR:
				logError(text);
			}
		}
	}
}