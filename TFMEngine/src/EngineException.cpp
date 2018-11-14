#include "EngineException.h"

#include "logger/Log.h"

namespace RenderLib
{
  EngineException::EngineException(const std::string & message) throw()
    : EngineException(message.c_str())
  {
  }

  EngineException::EngineException(const char * const message) throw()
    : std::runtime_error(message)
  {
    Logger::Log::getInstance().logError(std::string(message));
  }

  const char *
  EngineException::what() const throw()
  {
    return std::runtime_error::what();
  }
} // namespace RenderLib