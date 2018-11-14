#include "logger/Log.h"

namespace RenderLib
{
  namespace Logger
  {
    Log Log::INSTANCE;

    Log &
    Log::getInstance()
    {
      return INSTANCE;
    }

    std::string
    Log::getLevelString(LogLevel level)
    {
      std::string logStr;

      switch (level)
      {
        case LogLevel::LEVEL_ERROR:
          logStr = "[ERROR]";
          break;
        case LogLevel::LEVEL_WARNING:
          logStr = "[WARNING]";
          break;
        case LogLevel::LEVEL_INFO:
        default:
          logStr = "[INFO]";
          break;
      }

      return logStr;
    }

    void
    Log::log(const std::string & message, const LogLevel & level)
    {
      auto currentTime = std::chrono::system_clock::now();
      std::time_t currentDate
          = std::chrono::system_clock::to_time_t(currentTime);
      char dateStrBuf[0xff];

      std::tm time;
      LOCALTIME(time, &currentDate);

      std::strftime(dateStrBuf, 0xff, "[%d-%m-%Y_%H-%M_%S]", &time);
      std::string finalStr
          = std::string(dateStrBuf) + getLevelString(level) + message;

      std::unique_lock<std::mutex> lock(mtx);
      for (auto & logger : loggers)
      {
        if (logger.get() != NULL)
        {
          logger.get()->log(finalStr, level);
        }
      }
      lock.unlock();
    }

    void
    Log::logError(const std::string & error)
    {
      auto currentTime = std::chrono::system_clock::now();
      std::time_t currentDate
          = std::chrono::system_clock::to_time_t(currentTime);
      char dateStrBuf[0xff];

      std::tm time;
      LOCALTIME(time, &currentDate);

      std::strftime(dateStrBuf, 0xff, "[%d-%m-%Y_%H-%M_%S][ERROR]", &time);
      std::string finalStr = std::string(dateStrBuf) + error;

      std::unique_lock<std::mutex> lock(mtx);
      for (auto & logger : loggers)
      {
        if (logger.get() != NULL)
        {
          logger.get()->logError(finalStr);
        }
      }
      lock.unlock();
    }

    void
    Log::logInfo(const std::string & info)
    {
      auto currentTime = std::chrono::system_clock::now();
      std::time_t currentDate
          = std::chrono::system_clock::to_time_t(currentTime);
      char dateStrBuf[0xff];

      std::tm time;
      LOCALTIME(time, &currentDate);

      std::strftime(dateStrBuf, 0xff, "[%d-%m-%Y_%H-%M_%S][INFO]", &time);
      std::string finalStr = std::string(dateStrBuf) + info;

      std::unique_lock<std::mutex> lock(mtx);
      for (auto & logger : loggers)
      {
        if (logger.get() != NULL)
        {
          logger.get()->logInfo(finalStr);
        }
      }
      lock.unlock();
    }

    void
    Log::logWarning(const std::string & warning)
    {
      auto currentTime = std::chrono::system_clock::now();
      std::time_t currentDate
          = std::chrono::system_clock::to_time_t(currentTime);
      char dateStrBuf[0xff];

      std::tm time;
      LOCALTIME(time, &currentDate);

      std::strftime(dateStrBuf, 0xff, "[%d-%m-%Y_%H-%M_%S][WARNING]", &time);
      std::string finalStr = std::string(dateStrBuf) + warning;

      std::unique_lock<std::mutex> lock(mtx);
      for (auto & logger : loggers)
      {
        if (logger.get() != NULL)
        {
          logger.get()->logWarning(finalStr);
        }
      }
      lock.unlock();
    }

    Log::Log()
    {
    }

    Log::~Log()
    {
      /*
			for (auto & logger : loggers)
			{
				logger.reset();
			}
			*/
    }
  } // namespace Logger
} // namespace RenderLib