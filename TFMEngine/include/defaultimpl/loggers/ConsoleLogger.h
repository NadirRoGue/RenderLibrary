#ifndef __RENDERLIB_DEFAULTIMPL_CONSOLELOGGER_H__
#define __RENDERLIB_DEFAULTIMPL_CONSOLELOGGER_H__

#include "logger/AbstractLogger.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class ConsoleLogger : public Logger::AbstractLogger
    {
    public:
      void
      logInfo(const std::string & info);
      void
      logWarning(const std::string & warn);
      void
      logError(const std::string & error);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
