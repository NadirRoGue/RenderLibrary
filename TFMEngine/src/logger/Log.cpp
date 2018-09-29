#include "logger/Log.h"

namespace RenderLib
{
	namespace Logger
	{
		std::unique_ptr<Log> Log::INSTANCE = std::make_unique<Log>();


		Log & Log::getInstance()
		{
			return *(INSTANCE.get());
		}

		void Log::log(const std::string & message, const LogLevel & level)
		{
			if (logger.get() != NULL)
			{
				logger.get()->log(message, level);
			}
		}

		void Log::logError(const std::string & error)
		{
			if (logger.get() != NULL)
			{
				logger.get()->logError(error);
			}
		}

		void Log::logInfo(const std::string & info)
		{
			if (logger.get() != NULL)
			{
				logger.get()->logInfo(info);
			}
		}

		void Log::logWarning(const std::string & warning)
		{
			if (logger.get() != NULL)
			{
				logger.get()->logWarning(warning);
			}
		}

		Log::Log()
		{
		}

		Log::~Log()
		{
			logger.reset();
		}
	}
}