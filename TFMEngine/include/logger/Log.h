#ifndef __RENDERLIB_LOGGER_LOG_H__
#define __RENDERLIB_LOGGER_LOG_H__

#include <memory>
#include <mutex>
#include <vector>

#include "AbstractLogger.h"

#include <iostream>

namespace RenderLib
{
  namespace Logger
  {
    class Log
    {
    private:
      static Log INSTANCE;

    private:
      std::vector<std::unique_ptr<AbstractLogger>> loggers;
      std::mutex mtx;

    public:
      Log();
      ~Log();

    public:
      static Log &
      getInstance();

    public:
      template <class T>
      void
      addLogger()
      {
        if (std::is_base_of<AbstractLogger, T>::value)
        {
          for (auto & cl : loggers)
          {
            // A log of the given type exists, skip it
            if (dynamic_cast<T *>(cl.get()) != NULL)
            {
              std::cerr << "Log: Skipping adding duplicate logger of type "
                        << typeid(T).name() << std::endl;
              return;
            }
          }

          std::unique_ptr<AbstractLogger> newLogger = std::make_unique<T>();
          loggers.push_back(std::move(newLogger));
        }
        else
        {
          logError("Log: Attempted to change logger with a non AbstractLogger "
                   "derived class");
        }
      }

      void
      log(const std::string & text, const LogLevel & level);
      void
      logInfo(const std::string & text);
      void
      logWarning(const std::string & text);
      void
      logError(const std::string & text);

    private:
      std::string
      getLevelString(LogLevel level);
    };
  } // namespace Logger
} // namespace RenderLib

#endif