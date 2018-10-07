#include "defaultimpl/loggers/BufferedFileLogger.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		BufferedFileLogger::BufferedFileLogger()
			: FileLogger()
		{
			bufferInfo.reserve(LOG_QUEUE_SIZE);
			bufferWarning.reserve(LOG_QUEUE_SIZE);
			bufferError.reserve(LOG_QUEUE_SIZE);
		}

		BufferedFileLogger::~BufferedFileLogger()
		{
			if (bufferInfo.size() > 0)
			{
				FileLogger::logInfo(buildLogString(bufferInfo));
			}
			if (bufferWarning.size() > 0)
			{
				FileLogger::logWarning(buildLogString(bufferWarning));
			}
			if (bufferError.size() > 0)
			{
				FileLogger::logError(buildLogString(bufferError));
			}
		}

		std::string BufferedFileLogger::buildLogString(std::vector<std::string> & buffer)
		{
			std::string result = "";
			for (size_t i = 0; i < buffer.size() - 1; i++)
			{
				result += buffer[i] + "\n";
			}

			result += buffer[buffer.size() - 1];

			return result;
		}

		void BufferedFileLogger::logInfo(const std::string & info)
		{
			bufferInfo.push_back(info);
			if (bufferInfo.size() >= LOG_QUEUE_SIZE)
			{
				std::string infoStr = buildLogString(bufferInfo);
				bufferInfo.clear();
				FileLogger::logInfo(infoStr);
			}
		}

		void BufferedFileLogger::logWarning(const std::string & warn)
		{
			bufferWarning.push_back(warn);
			if (bufferWarning.size() >= LOG_QUEUE_SIZE)
			{
				std::string warnStr = buildLogString(bufferWarning);
				bufferWarning.clear();
				FileLogger::logWarning(warnStr);
			}
		}

		void BufferedFileLogger::logError(const std::string & error)
		{
			bufferError.push_back(error);
			if (bufferError.size() >= LOG_QUEUE_SIZE)
			{
				std::string errorStr = buildLogString(bufferError);
				bufferError.clear();
				FileLogger::logInfo(errorStr);
			}
		}
	}
}