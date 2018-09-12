#include "logger/Log.h"

#include "logger/ConsoleLogger.h"

namespace RenderLib
{
	namespace Logger
	{
		std::unique_ptr<Log> Log::INSTANCE = std::make_unique<Log>();


		Log & Log::getInstance()
		{
			return *(INSTANCE.get());
		}

		void Log::setLogger(std::unique_ptr<AbstractLogger> && newLogger)
		{
			logger.reset();
			logger = std::move(newLogger);
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
			setLogger(std::make_unique<ConsoleLogger>());
		}

		Log::~Log()
		{
			logger.reset();
		}
	}
}